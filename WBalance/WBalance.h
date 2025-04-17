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

    //系数校准
    QString glist_path;          //加载表文件路径                  
    QString load_path;           //载荷文件路径                  
    QString voltage_path;        //电压文件路径
    QString loadSD_path;         //载荷标准差路径                  
    QString voltageSD_path;      //电压标准差路径
    QStringList load_header;     //载荷表头
    QStringList voltage_header;  //电压表头

    bool glist_flag;             //打开加载表
    bool load_flag;              //打开载荷文件
    bool voltage_flag;           //打开电压文件
    bool data_confirm_flag;      //确认数据文件
    bool loadSD_flag;            //打开载荷标准差
    bool voltageSD_flag;         //打开电压标准差
    bool sd_confirm_flag;        //确认标准差文件
    bool saveornot;

    vector<vector<int>> zerolines;         //加载表的零行索引
    MatrixXd f_matrix;                     //载荷,144*6
    MatrixXd u_matrix;                     //电压,144*6
    vector<vector<double>> zero_rows_u_0;  //修正值,6*10
    vector<MatrixXd> delta_u_0_list;       //修正结果,6*144
    vector<MatrixXd> A_matrix;             //自变量矩阵,6*144*27
    MatrixXd f_matrixSD;                   //载荷标准差,144*6
    MatrixXd u_matrixSD;                   //电压标准差,144*6

    struct file_data 
    {
        bool ok; //是否读取成功
        QStringList headers; //表头
        MatrixXd data; //数据
    };

    void clearModelData(QTableView *tableView);          //清空tableview
    QString OpenFile(QLineEdit *lineEdit, QString type); //打开文件
    file_data read_file(const QString &path);            //提取文件数据
    void show_data(const QStringList &header, const MatrixXd &matri, QTableView *tableView); //表格显示数据

    MatrixXd read_tableview(QTableView *tableView); //读取表格数据
    QPair<bool, vector<vector<int>>> find_zeroRows_index(const QString &glistFilePath);           //搜索加载表中的零行
    QPair<vector<vector<double>>, vector<MatrixXd>> correct_voltage(const MatrixXd &vol);         //修正电压
    vector<MatrixXd> compute_inde_variable(const vector<MatrixXd> &delta_u_i, const MatrixXd &f); //计算自变量矩阵
    MatrixXd compute_coefficient_Y(const vector<MatrixXd> &ai, const MatrixXd &f);                //等精度最小二乘计算系数
    void save_calibra_result(const QString &Path); //保存校准过程结果

    //系数评估，从144*12每一个数据的分布中抽样
    vector<MatrixXd> voltage_sample_result;  //电压抽样结果，M*144*6
    vector<MatrixXd> load_sample_result;     //载荷抽样结果，M*144*6
    MatrixXd coeff_average;                  //估计值，27*6
    MatrixXd coeff_error;                    //不确定度，27*6

    QPair<vector<MatrixXd>, vector<MatrixXd>> data_sample(); //抽样
    QPair<MatrixXd, MatrixXd> cal_sample_results(const vector<MatrixXd> &vol, const vector<MatrixXd> &load); //计算估计值和标准不确定度
    bool save_coeff_file(const QString& path, const MatrixXd& data, const QString& title, const QStringList& headers);

    QString read_data00();
    double ThirdRule(double data);

    void read_load01_data();
    void read_data01_data();
    void show_data01_load01();//显示综合加载误差数据
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
    void closeEvent(QCloseEvent *event) override; //重写关闭事件

private slots:
    void show_choosefile();       //打开文件界面
    void show_data_analyse();     //数据分析界面
    void show_oncalibration();    //模型校准界面
    void quit();                  //退出
    void help();                  //打开帮助文档

    void SelectFile_glist();      //打开加载表文件
    void SelectFile_load();       //打开载荷文件
    void SelectFile_voltage();    //打开电压文件
    void read_data_cofirm();      //确认数据文件选择

    void SelectFile_loadSD();     //打开载荷标准差文件
    void SelectFile_voltageSD();  //打开电压标准差文件
    void read_sd_cofirm();        //确认标准差文件选择

    void coeff_evaluate();        //模型校准槽函数
    void save_calibration();      //保存校准结果

    void show_data00();           //重复性-读取data00文件
    void compute_repeat_result(); //重复性-评估
    void save_comprehen_cfx();    //重复性-结果保存

    void btnSaveClicked();        //输出报告
};

