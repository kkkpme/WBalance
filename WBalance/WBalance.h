#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_WBalance.h"
#include <Eigen/Dense>
#include <Eigen/Core>
#include <QAxObject>
#include <QCloseEvent>
using namespace std;
using namespace Eigen;

class WBalance : public QMainWindow
{
    Q_OBJECT

public:
    WBalance(QWidget *parent = nullptr);
    ~WBalance();

    //ϵ��У׼
    QString glist_path;          //���ر��ļ�·��                  
    QString load_path;           //�غ��ļ�·��                  
    QString voltage_path;        //��ѹ�ļ�·��
    QString loadSD_path;         //�غɱ�׼��·��                  
    QString voltageSD_path;      //��ѹ��׼��·��
    QStringList load_header;     //�غɱ�ͷ
    QStringList voltage_header;  //��ѹ��ͷ

    bool glist_flag;             //�򿪼��ر�
    bool load_flag;              //���غ��ļ�
    bool voltage_flag;           //�򿪵�ѹ�ļ�
    bool data_confirm_flag;      //ȷ�������ļ�
    bool loadSD_flag;            //���غɱ�׼��
    bool voltageSD_flag;         //�򿪵�ѹ��׼��
    bool sd_confirm_flag;        //ȷ�ϱ�׼���ļ�
    bool saveornot;

    vector<vector<int>> zerolines;         //���ر����������
    MatrixXd f_matrix;                     //�غ�,144*6
    MatrixXd u_matrix;                     //��ѹ,144*6
    vector<vector<double>> zero_rows_u_0;  //����ֵ,6*10
    vector<MatrixXd> delta_u_0_list;       //�������,6*144
    vector<MatrixXd> A_matrix;             //�Ա�������,6*144*27
    MatrixXd f_matrixSD;                   //�غɱ�׼��,144*6
    MatrixXd u_matrixSD;                   //��ѹ��׼��,144*6

    struct file_data 
    {
        bool ok; //�Ƿ��ȡ�ɹ�
        QStringList headers; //��ͷ
        MatrixXd data; //����
    };

    void clearModelData(QTableView *tableView);          //���tableview
    QString OpenFile(QLineEdit *lineEdit, QString type); //���ļ�
    file_data read_file(const QString &path);            //��ȡ�ļ�����
    void show_data(const QStringList &header, const MatrixXd &matri, QTableView *tableView); //�����ʾ����

    MatrixXd read_tableview(QTableView *tableView); //��ȡ�������
    QPair<bool, vector<vector<int>>> find_zeroRows_index(const QString &glistFilePath);           //�������ر��е�����
    QPair<vector<vector<double>>, vector<MatrixXd>> correct_voltage(const MatrixXd &vol);         //������ѹ
    vector<MatrixXd> compute_inde_variable(const vector<MatrixXd> &delta_u_i, const MatrixXd &f); //�����Ա�������
    MatrixXd compute_coefficient_Y(const vector<MatrixXd> &ai, const MatrixXd &f);                //�Ⱦ�����С���˼���ϵ��
    void save_calibra_result(const QString &Path); //����У׼���̽��

    //ϵ����������144*12ÿһ�����ݵķֲ��г���
    vector<MatrixXd> voltage_sample_result;  //��ѹ���������M*144*6
    vector<MatrixXd> load_sample_result;     //�غɳ��������M*144*6
    MatrixXd coeff_average;                  //����ֵ��27*6
    MatrixXd coeff_error;                    //��ȷ���ȣ�27*6

    QPair<vector<MatrixXd>, vector<MatrixXd>> data_sample(); //����
    QPair<MatrixXd, MatrixXd> cal_sample_results(const vector<MatrixXd> &vol, const vector<MatrixXd> &load); //�������ֵ�ͱ�׼��ȷ����
    bool save_coeff_file(const QString& path, const MatrixXd& data, const QString& title, const QStringList& headers);

    QString read_data00();
    double ThirdRule(double data);

    void read_load01_data();
    void read_data01_data();
    void show_data01_load01();//��ʾ�ۺϼ����������
    void compute_comprehen();
    void save_comprehen();

    void handleFileSelection(QLineEdit* lineEdit);
    QStringList readTextFile(const QString& filePath);
    void fillTableWithData(QAxObject* table, const QStringList& data, int startRow, int startCol);
    void insertTextByBookmark(QAxObject* document, const QString& bookmarkName, const QString& text);
    bool isNumeric(const QString& str);
    bool isFileLocked(const QString& filePath);

private:
    Ui::WBalanceClass ui;

protected:
    void closeEvent(QCloseEvent *event) override; //��д�ر��¼�

private slots:
    void show_choosefile();       //���ļ�����
    void show_data_analyse();     //���ݷ�������
    void show_oncalibration();    //ģ��У׼����
    void quit();                  //�˳�
    void help();                  //�򿪰����ĵ�

    void SelectFile_glist();      //�򿪼��ر��ļ�
    void SelectFile_load();       //���غ��ļ�
    void SelectFile_voltage();    //�򿪵�ѹ�ļ�
    void read_data_cofirm();      //ȷ�������ļ�ѡ��

    void SelectFile_loadSD();     //���غɱ�׼���ļ�
    void SelectFile_voltageSD();  //�򿪵�ѹ��׼���ļ�
    void read_sd_cofirm();        //ȷ�ϱ�׼���ļ�ѡ��

    void coeff_evaluate();        //ģ��У׼�ۺ���
    void save_calibration();      //����У׼���

    void show_data00();           //�ظ���-��ȡdata00�ļ�
    void compute_repeat_result(); //�ظ���-����
    void save_comprehen_cfx();    //�ظ���-�������

    void btnSaveClicked();        //�������
};

