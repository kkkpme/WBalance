#include "Worker.h"
#include <random>
#include <numeric>
#include <QtMath>
#include <QDebug>
#include <QPair>
#include <QThread>

Worker::Worker(QObject* parent)
    : QObject(parent)
{
}

Worker::Worker(vector<vector<int>> zerolines,
    const MatrixXd &f_matrix,
    const MatrixXd &f_matrixSD,
    const MatrixXd &u_matrix,
    const MatrixXd &u_matrixSD,
    const QStringList &loadHeader,
    int sampleCount,
    QObject* parent)
    : QObject(parent)
    , zero_lines(zerolines)
    , m_f(f_matrix)
    , m_fSD(f_matrixSD)
    , m_u(u_matrix)
    , m_uSD(u_matrixSD)
    , m_loadHeader(loadHeader)
    , m_numSamples(sampleCount)
{}

QPair<vector<MatrixXd>, vector<MatrixXd>> Worker::data_sample()
{
    int row = m_f.rows();
    int col = m_f.cols();

    vector<MatrixXd> vol_results, load_results;
    vol_results.reserve(m_numSamples);
    load_results.reserve(m_numSamples);

    random_device rd;
    mt19937 gen(rd());

    for (int k = 0; k < m_numSamples; ++k) 
    {
        if (QThread::currentThread()->isInterruptionRequested()) 
        {
            //��ʱ�˳�
            return {};
        }
        
        MatrixXd vol_temp_result = MatrixXd::Zero(row, col);
        MatrixXd load_temp_result = MatrixXd::Zero(row, col);
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                //�غ�
                if (m_fSD(i, j) == 0.0)
                {
                    load_temp_result(i, j) = m_f(i, j);
                }
                else
                {
                    normal_distribution<double> load_normal_dist(m_f(i, j), fabs(m_fSD(i, j)));
                    load_temp_result(i, j) = load_normal_dist(gen);
                }
                //��ѹ
                if (m_uSD(i, j) == 0.0)
                {
                    vol_temp_result(i, j) = m_u(i, j);
                }
                else 
                {
                    normal_distribution<double> vol_normal_dist(m_u(i, j), fabs(m_uSD(i, j)));
                    vol_temp_result(i, j) = vol_normal_dist(gen);
                }
            }
        }

        if ((k + 1) % (10000) == 0) 
        {
            int value = (k + 1) / 10000;
            qDebug() << "value=" << value;
            emit Progress(value);
        }
        vol_results.push_back(move(vol_temp_result));
        load_results.push_back(move(load_temp_result));
    }
    emit Progress(10);
    return qMakePair(move(vol_results), move(load_results));
}

QPair<vector<vector<double>>, vector<MatrixXd>> Worker::correct_voltage(const MatrixXd& vol)
{
    //--25.3.4�޸�--���������غ��м��������ֵ��ȥ��������֮�����е�������
    int rowCount = 144;
    int colCount = 6;
    //����ֵ
    int num = zero_lines.size(); //���غ��к������
    vector<vector<double>> zero_u_0;
    zero_u_0.clear();
    zero_u_0 = vector<vector<double>>(colCount, vector<double>(num, 0.0)); //6*num������ֵ

    for (int i = 0; i < colCount; i++)
    {
        for (int j = 0; j < num; j++)
        {
            double sum = 0.0;
            for (int k : zero_lines[j])
            {
                sum += vol(k, i);
            }
            zero_u_0[i][j] = sum / zero_lines[j].size();
        }
    }

    //�������
    vector<MatrixXd> u_0_list;
    u_0_list.clear();
    int matNum = vol.cols();

    for (int i = 0; i < matNum; i++) //6�д�������ѹ
    {
        MatrixXd mat = vol.col(i);
        MatrixXd delta_u_0(rowCount, 1);    //144*1
        for (int j = 0; j < num; j++)       //ÿ����num������ֵ����Ҫ�ֳ�num��ֱ��ȥ��Ӧ����ֵ
        {
            vector<int> zero = zero_lines[j];
            for (int k = zero[0]; k <= zero[1]; k++)
            {
                delta_u_0(k, 0) = mat(k, 0) - zero_u_0[i][j];
            }
        }
        u_0_list.push_back(delta_u_0); //������õ�u_0_list����6��144�е�������
    }

    return QPair<vector<vector<double>>, vector<MatrixXd>>(zero_u_0, u_0_list); //��������ֵ+�������
}

vector<MatrixXd> Worker::compute_inde_variable(const vector<MatrixXd>& delta_u_i, const MatrixXd& f)
{
    int rowCount = delta_u_i[0].size();

    vector<MatrixXd> py_one_infer;//һ�׸�����

    for (int i = 0; i < f.cols(); i++)
    {
        MatrixXd a = f;
        a.col(i) = delta_u_i[i]; //��delta_u_0_list��˳���滻��a�е�һ��
        py_one_infer.push_back(a); //�õ�6��144*6����
    }

    MatrixXd py_two_infer(rowCount, 21); //���׸����������
    for (int i = 0; i < 6; i++) //py_two_inferǰ6��У���f��ÿһ��ÿ��Ԫ�ص�ƽ��
    {
        py_two_infer.col(i) = f.col(i).array().square();
    }

    int index = 6;
    for (int i = 0; i < 5; i++)//py_two_infer��15��У�����i��ÿ��Ԫ�غ͵�j��>i����ÿ��Ԫ�����
    {
        for (int j = i + 1; j < 6; j++)
        {
            py_two_infer.col(index++) = f.col(i).array() * f.col(j).array();
        }
    }

    vector<MatrixXd> Ai; //�Ա�������
    for (int i = 0; i < 6; i++)
    {
        MatrixXd a(rowCount, py_one_infer[i].cols() + py_two_infer.cols()); //��ʼ��144*27����
        a << py_one_infer[i], py_two_infer; //ƴ�Ӿ���[py_one_infer[i],py_two_infer]
        Ai.push_back(a); //A��6��144*27����
    }
    return Ai;
}

MatrixXd Worker::compute_coefficient_Y(const vector<MatrixXd>& ai, const MatrixXd& f)
{
    vector<MatrixXd> X; //ϵ������
    for (int i = 0; i < 6; i++)
    {
        MatrixXd a = (ai[i].transpose() * ai[i]).inverse() * ai[i].transpose() * f.col(i);
        X.push_back(a);
    }

    int rowCount = X[0].rows();//27
    int colCount = X[0].cols();//1
    MatrixXd mtx = MatrixXd::Zero(rowCount, colCount * X.size());

    int colIdx = 0;
    for (const auto& mat : X)
    {
        mtx.block(0, colIdx, rowCount, colCount) = mat; //����mtx�е�ÿһ������mat������������ƴ�ӵ�mtx�У����õ�һ��27*6�ľ���
        colIdx++;
    }

    return mtx;
}

QPair<MatrixXd, MatrixXd> Worker::cal_sample_results(const vector<MatrixXd>& vol, const vector<MatrixXd>& load)
{
    int M = int(vol.size());
    vector<MatrixXd> coeffs(M, MatrixXd::Zero(27, 6));

    MatrixXd vol_matri;
    MatrixXd load_matri;

    for (int i = 0; i < M; ++i) 
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            //��ʱ�˳�
            return {};
        }
        
        vol_matri = vol[i];
        load_matri = load[i];

        auto m = correct_voltage(vol_matri);  //������ѹ����������ֵ���������
        zero_rows_u_0 = m.first;              //��ѹ����ֵ
        delta_u_0_list = m.second;            //��ѹ�������

        vector<MatrixXd> a_i = compute_inde_variable(delta_u_0_list, load_matri); //�����ѹ+�غ�		
        MatrixXd coeff = compute_coefficient_Y(a_i, load_matri);//�Ⱦ��ȼ���
        coeffs[i] = coeff;

        //����ÿ1149�θ���һ��
        if (i % 1149 == 0) 
        {
            int value = i / 1149 + 10;
            qDebug() << "value=" << value;
            emit Progress(value);
        }
    }

    //QString name = "D:/M��ϵ��.txt";
    //QFile file(name);
    //if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    //{
    //	qDebug() << "�޷����ļ���" << name;
    //	return QPair<MatrixXd, MatrixXd>();
    //}

    //QTextStream out(&file);

    //for (const auto& matrices : coeffs)
    //{
    //	int matrixIndex = 0;

    //	for (int i = 0; i < matrices.rows(); i++)
    //	{
    //		for (int j = 0; j < matrices.cols(); j++)
    //		{
    //			out << QString::number(matrices(i, j), 'f', 12) << " ";
    //		}
    //		out << "\n";
    //	}
    //}

    //file.close();
    //qDebug() << "ϵ���ѱ��浽��" << name;

    MatrixXd averg = MatrixXd::Zero(27, 6);
    MatrixXd error = MatrixXd::Zero(27, 6);

    for (int i = 0; i < M; i++)
    {
        //M��ϵ������
        MatrixXd mat = coeffs[i];
        averg += mat;
    }
    averg = averg / M; //ƽ��ֵ

    emit Progress(98);

    for (int i = 0; i < M; i++) //�в��
    {
        MatrixXd vi = (coeffs[i] - averg).array().square();
        error += vi;
    }
    
    //��׼��ȷ����
    if (M <= 1)
    {
        qDebug() << "������С��1���޷������׼��ȷ����";
        return QPair<MatrixXd, MatrixXd>();
    }
    for (int i = 0; i < error.rows(); i++)
    {
        for (int j = 0; j < error.cols(); j++)
        {
            error(i, j) = qSqrt(error(i, j) / (M - 1));
        }
    }

    emit Progress(99);

    return qMakePair(averg, error);
}

void Worker::process()
{
    try
    {
        auto samples = data_sample();
        if (QThread::currentThread()->isInterruptionRequested()) 
        {
            emit cancelled();
            return;
        }
        emit Progress(10);

        auto results = cal_sample_results(samples.first, samples.second);
        if (QThread::currentThread()->isInterruptionRequested()) 
        {
            emit cancelled();
            return;
        }
        emit Progress(99);

        emit finished(results.first, results.second);
    }
    catch (std::exception& ex)
    {
        emit errorOccured(QString("��̨�������%1").arg(ex.what()));
    }
}

Worker::~Worker()
{}
