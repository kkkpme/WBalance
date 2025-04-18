#pragma once

#include <QObject>
#include <Eigen/Dense>
#include <vector>
#include <QMetaType>

using Eigen::MatrixXd;
using namespace std;

Q_DECLARE_METATYPE(Eigen::MatrixXd)

class Worker : public QObject 
{
    Q_OBJECT

public:

    Worker(QObject* parent);
    ~Worker();

    Worker(vector<vector<int>> zerolines,
        const MatrixXd& f_matrix,
        const MatrixXd& f_matrixSD,
        const MatrixXd& u_matrix,
        const MatrixXd& u_matrixSD,
        const QStringList& loadHeader,
        int sampleCount = 100000,
        QObject* parent = nullptr);


    //����+�������������
    vector<vector<int>> zero_lines;
    vector<vector<double>> zero_rows_u_0;  //����ֵ,6*10
    vector<MatrixXd> delta_u_0_list;       //�������,6*144

    MatrixXd m_f, m_fSD, m_u, m_uSD;
    QStringList m_loadHeader;
    int m_numSamples;

    QPair<vector<MatrixXd>, vector<MatrixXd>> data_sample();

    QPair<vector<vector<double>>, vector<MatrixXd>> correct_voltage(const MatrixXd& vol);         //������ѹ
    vector<MatrixXd> compute_inde_variable(const vector<MatrixXd>& delta_u_i, const MatrixXd& f); //�����Ա�������
    MatrixXd compute_coefficient_Y(const vector<MatrixXd>& ai, const MatrixXd& f);                //�Ⱦ�����С���˼���ϵ��

    QPair<MatrixXd, MatrixXd> cal_sample_results(const vector<MatrixXd>& vol, const vector<MatrixXd>& load);


public slots:
    //�߳���������Զ�����
    void process();

signals:
   
    void Progress(int value);    
   
    //ȫ����ɣ�����ƽ��ϵ���ͱ�׼��ȷ����
    void finished(Eigen::MatrixXd averageCoeff, Eigen::MatrixXd errorCoeff);
    void errorOccured(const QString& errMsg);
    void cancelled();

private:

};