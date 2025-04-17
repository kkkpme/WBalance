#pragma execution_character_set("utf-8")

#include "WBalance.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QBrush>
#include <QColor>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QBuffer>
#include <QDebug>
#include <locale>
#include <codecvt>
#include <QDir>
#include <QDateTime>
#include <QtMath>
#include <map>
#include <QPair>
#include <random>
#include <numeric>
#include <QDesktopServices>
#include "diedai.h"


#include <QTextCodec>
#include <QCoreApplication>
#include <QPushButton>
#include <QFileInfo>
#include <QProgressBar>
#include <QTextStream>

WBalance::WBalance(QWidget *parent)
    : QMainWindow(parent)
{	
    ui.setupUi(this);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    setWindowTitle("天平载荷校准和测量评价软件");
    ui.stackedWidget->setCurrentIndex(0);//默认显示数据读取页面

	glist_path = load_path = voltage_path = ""; //初始化路径为空
	glist_flag = false;
	load_flag = false;
	voltage_flag = false;
	data_confirm_flag = false;
	loadSD_flag = false;
	voltageSD_flag = false;
	sd_confirm_flag = false;
	saveornot = true;

	zerolines.clear();
	zero_rows_u_0.clear();
	delta_u_0_list.clear();
	A_matrix.clear();

	connect(ui.action_evaluate_repeat, &QAction::triggered, [=]()//	切换综合加载重复性数据读取界面
	{
		ui.stackedWidget->setCurrentIndex(3);
		ui.lineEdit_b_type_7->setText(ui.comboBox_b_type->currentText());
		ui.lineEdit_b_name_6->setText(ui.lineEdit_b_name->text().trimmed());
		ui.lineEdit_b_instru_6->setText(ui.lineEdit_b_instru->text().trimmed());
		ui.lineEdit_b_date_6->setText(ui.lineEdit_b_date->text().trimmed());
		ui.lineEdit_b_person_6->setText(ui.lineEdit_b_person->text().trimmed());
	});

	connect(ui.action_repeat, &QAction::triggered, [=]()//切换综合加载重复性评估界面
		{
			ui.stackedWidget->setCurrentIndex(4);
			ui.lineEdit_b_type_9->setText(ui.comboBox_b_type->currentText());
			ui.lineEdit_b_name_9->setText(ui.lineEdit_b_name->text().trimmed());
			ui.lineEdit_b_instru_9->setText(ui.lineEdit_b_instru->text().trimmed());
			ui.lineEdit_b_date_9->setText(ui.lineEdit_b_date->text().trimmed());
			ui.lineEdit_b_person_9->setText(ui.lineEdit_b_person->text().trimmed());
		});

	connect(ui.pushButton_read_coff_00, &QPushButton::clicked, [=]() //另选按钮
		{
			QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"));
			if (!filePath.isEmpty())
			{
				ui.lineEdit_coff_path->setText(filePath);
			}
			else
			{
				QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
				return;
			}
		});

	connect(ui.pushButton_show_data00, &QPushButton::clicked, [=]()//切换综合加载重复性评估界面
		{
			ui.stackedWidget->setCurrentIndex(4);
			ui.lineEdit_b_type_9->setText(ui.comboBox_b_type->currentText());
			ui.lineEdit_b_name_9->setText(ui.lineEdit_b_name->text().trimmed());
			ui.lineEdit_b_instru_9->setText(ui.lineEdit_b_instru->text().trimmed());
			ui.lineEdit_b_date_9->setText(ui.lineEdit_b_date->text().trimmed());
			ui.lineEdit_b_person_9->setText(ui.lineEdit_b_person->text().trimmed());
		});

	connect(ui.pushButton_read_vol_00, &QPushButton::clicked, this, &WBalance::show_data00);
	connect(ui.pushButton_compute00_result, &QPushButton::clicked, this, &WBalance::WBalance::compute_repeat_result);
	connect(ui.pushButton_save00_result, &QPushButton::clicked, this, &WBalance::WBalance::save_comprehen_cfx);

	connect(ui.action_analyse_erro, &QAction::triggered, [=]()//	切换综合加载误差数据读取界面
		{
			ui.stackedWidget->setCurrentIndex(5);
			ui.lineEdit_b_type_6->setText(ui.comboBox_b_type->currentText());
			ui.lineEdit_b_name_7->setText(ui.lineEdit_b_name->text().trimmed());
			ui.lineEdit_b_instru_7->setText(ui.lineEdit_b_instru->text().trimmed());
			ui.lineEdit_b_date_7->setText(ui.lineEdit_b_date->text().trimmed());
			ui.lineEdit_b_person_7->setText(ui.lineEdit_b_person->text().trimmed());


		});

	connect(ui.action_evaluate_error, &QAction::triggered, [=]()//	切换综合加载误差数据评估界面
		{
			ui.stackedWidget->setCurrentIndex(6);
			ui.lineEdit_b_type_4->setText(ui.comboBox_b_type->currentText());
			ui.lineEdit_b_name_5->setText(ui.lineEdit_b_name->text().trimmed());
			ui.lineEdit_b_instru_5->setText(ui.lineEdit_b_instru->text().trimmed());
			ui.lineEdit_b_date_5->setText(ui.lineEdit_b_date->text().trimmed());
			ui.lineEdit_b_person_5->setText(ui.lineEdit_b_person->text().trimmed());
		});

	connect(ui.pushButton_read_coff_2, &QPushButton::clicked, [=]() //另选按钮
		{
			QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath());
			if (!filePath.isEmpty())
			{
				ui.lineEdit_coff_path_2->setText(filePath);
			}
			else
			{
				QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
				return;
			}

		});

	connect(ui.pushButton_read_load01,&QPushButton::clicked,this,&WBalance::read_load01_data);
	connect(ui.pushButton_read_data01,&QPushButton::clicked,this,&WBalance::read_data01_data);
	connect(ui.pushButton_show_data01_load01, &QPushButton::clicked, this, &WBalance::show_data01_load01);
	connect(ui.pushButton_data_comprehen_ass, &QPushButton::clicked, this, &WBalance::compute_comprehen);
	connect(ui.pushButton_data_comprehen_save, &QPushButton::clicked, this, &WBalance::save_comprehen);


	connect(ui.action_report, &QAction::triggered, [=]()//	切换输出报告界面
		{
			ui.stackedWidget->setCurrentIndex(7);
			ui.lineEdit_b_type_8->setText(ui.comboBox_b_type->currentText());
			ui.lineEdit_b_name_8->setText(ui.lineEdit_b_name->text().trimmed());
			ui.lineEdit_b_instru_8->setText(ui.lineEdit_b_instru->text().trimmed());
			ui.lineEdit_b_date_8->setText(ui.lineEdit_b_date->text().trimmed());
			ui.lineEdit_b_person_8->setText(ui.lineEdit_b_person->text().trimmed());


			ui.progressBar->setRange(0, 100);
			ui.progressBar->setVisible(false);
			ui.TableWidget_b->setHorizontalHeaderLabels({ "Y", "Mz", "Mx", "X", "Z", "My" });
			ui.TableWidget_b->setVerticalHeaderLabels({ "设计载荷", "校准载荷" });			

		});

	connect(ui.pushButton_choose_EV, &QPushButton::clicked, this,
		[this]() { handleFileSelection(ui.lineEdit_EV_save_path); });

	connect(ui.pushButton_choose_SUEV, &QPushButton::clicked, this,
		[this]() { handleFileSelection(ui.lineEdit_SUEV_save_path); });

	//待马璇师姐完成后加入
	/*connect(ui.pushButton_choose_UCLR, &QPushButton::clicked, this,
		[this]() { handleFileSelection(ui.lineEdit_UCLR_save_path); });*/

	connect(ui.pushButton_choose_CLE, &QPushButton::clicked, this,
		[this]() { handleFileSelection(ui.lineEdit_CLE_save_path); });

	connect(ui.pushButton_report_save, &QPushButton::clicked, this, &WBalance::btnSaveClicked);

}


WBalance::~WBalance()
{}

void WBalance::clearModelData(QTableView *tableView)
{
	if (tableView->model())
	{
		if (QStandardItemModel *standardModel = qobject_cast<QStandardItemModel*>(tableView->model()))
		{
			standardModel->clear();
		}
	}
	tableView->update();
}

QString WBalance::OpenFile(QLineEdit *lineEdit, QString type)
{
	type = QString("选择") + type + QString("文件");
    QString filename = QFileDialog::getOpenFileName(this, type);
	qDebug() << "filename: " << filename;
    if (!filename.isEmpty())
    {
		clearModelData(ui.tableView_cofe_averg);
		clearModelData(ui.tableView_cofe_std);
		lineEdit->setText(filename);
    }
	else
	{
		QMessageBox::information(this, tr("提示"), tr("请选择文件！"));
	}
    return filename;
}

WBalance::file_data WBalance::read_file(const QString &path)
{
	file_data result;  //定义返回结构体
	result.ok = false; //默认状态为失败

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("错误!"), tr("无法打开文件: ") + file.errorString());
		return result;
	}

	QTextStream in(&file);
	in.setCodec("UTF-8");

	//1. 读取第一行表头，并按空白字符分割
	QString headerLine = in.readLine();
	QStringList headers = headerLine.split(QRegularExpression("[,、\\s]+"), Qt::SkipEmptyParts);
	qDebug() << headers;

	if (headers.isEmpty())
	{
		QMessageBox::critical(this, tr("提示!"), tr("表头为空"));
		file.close();
		return result;
	}

	result.headers = headers;
	int colCount = headers.size(); //列数，由表头决定

	//2. 读取剩余数据行
	vector<QString> dataLines;
	while (!in.atEnd())
	{
		QString line = in.readLine(); //将文件的每一line读入lines数组
		if (!line.trimmed().isEmpty())
			dataLines.push_back(line);
	}
	file.close();

	//3. 处理数据
	int rowCount = dataLines.size(); //行数
	MatrixXd rawData = MatrixXd::Zero(rowCount, colCount);

	//用正则表达式提取数值。这里使用能匹配科学计数法、小数或整数的正则表达式
	QRegularExpression numPattern(R"((-?\d+\.?\d*([eE][+-]?\d+)?|\.\d+([eE][+-]?\d+)?))");

	//逐行处理：跳过第一个匹配（行号），后面的依次作为数据
	for (int i = 0; i < rowCount; ++i)
	{
		const QString& line = dataLines[i];
		QRegularExpressionMatchIterator it = numPattern.globalMatch(line);

		//第一个匹配为行号，跳过
		if (it.hasNext()) it.next();

		int j = 0;
		while (it.hasNext() && j < colCount)
		{
			QRegularExpressionMatch match = it.next();
			QString token = match.captured(0);
			rawData(i, j) = token.toDouble();
			++j;
		}
		if (j != colCount)
		{
			qDebug() << "第" << i << "行数据提取不足，预期" << colCount << "个数据，实际" << j;
		}
	}

	//4. 返回成功结果
	result.ok = true;
	result.data = rawData;
	return result;
}

 MatrixXd WBalance::read_tableview(QTableView *tableView)
{
	//读取表格数据
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(tableView->model());
	if (!model) {
		qWarning() << "Invalid table model";
		return MatrixXd(0, 0);
	}

	const int rowCount = model->rowCount();
	const int colCount = model->columnCount();

	Eigen::MatrixXd matrix(rowCount, colCount);

	for (int i = 0; i < rowCount; ++i) 
	{
		for (int j = 0; j < colCount; ++j) 
		{
			QStandardItem* item = model->item(i, j);
			if (!item) 
			{
				QMessageBox::critical(this, tr("错误"), tr("表格中含有空单元格！"));
				return MatrixXd(0, 0);
			}
			else 
			{
				//科学计数法字符串转浮点数
				QString strValue = item->text();
				bool ok;
				double value = strValue.toDouble(&ok);
				matrix(i, j) = value;
			}
		}
	}
	return matrix;
}

QPair<bool, vector<vector<int>>> WBalance::find_zeroRows_index(const QString &glistFilePath)
{
	vector<vector<int>> zero_rows;//存放载荷为0的行的索引
	QFile file(glistFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("错误"), tr("无法打开加载表文件：") + file.errorString());
		return QPair<bool, vector<vector<int>>>(false, zero_rows);
	}

	QTextStream in(&file);
	vector<QString> lines;//将文件中内容读入lines数组
	while (!in.atEnd())
	{
		QString line = in.readLine(); //逐行读取,存放文件的一行内容
		lines.push_back(line);
	}
	file.close();

	lines.erase(lines.begin(), lines.begin() + 1); //删除表头

	QRegularExpression  pattern(R"(^\s*\d{1,3}\s+)"); //匹配每一行的行号
	for (auto &l : lines)
	{
		l = l.replace(pattern, "");//删除每一行的行号
	}

	auto check_zero_row = [](const QString &line) -> bool
		{
			QStringList values = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
			for (const QString& value : values)
			{
				bool ok;
				if (value.toDouble(&ok) != 0 || !ok)
				{
					return false; //如果有任何值不为0，返回false
				}
			}
			return true; //如果全为0，则返回true
		};

	/*
	修改分组方式：按顺序搜索零载荷行号，每找到2个，构成一个组合zero_row，将所有组合存放在zero_rows中
	*/
	vector<int> zero_row;
	int same = 0;
	for (int index = 0; index < lines.size(); index++)
	{
		if (check_zero_row(lines[index]))
		{
			zero_row.push_back(index);
			same++;
		}
		if (same == 2)
		{
			zero_rows.push_back(zero_row);
			same = 0;
			zero_row.clear();
		}
	}

	if (zero_rows.size() == 0)
	{
		QMessageBox::information(this, tr("提示"), tr("未找到零行载荷！"));
		return QPair<bool, vector<vector<int>>>(false, zero_rows);
	}

	data_confirm_flag = false;
	QMessageBox::information(this, tr("提示"), tr("加载表文件读取成功"));
	return QPair<bool, vector<vector<int>>>(true, zero_rows);
}

void WBalance::show_data(const QStringList &header, const MatrixXd &matri, QTableView *tableView)
{
	int rowCount = matri.rows();
	int colCount = matri.cols();

	QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, this);
	model->setHorizontalHeaderLabels(header);
	for (int i = 0; i < rowCount; ++i)
	{
		for (int j = 0; j < colCount; ++j)
		{
			QString snValue = QString::number(matri(i, j), 'f', 8).replace(QRegularExpression("\\.?0+$"), "");
			QStandardItem *item = new QStandardItem(snValue);
			model->setItem(i, j, item);
		}
	}

	for (const vector<int>& group : zerolines)
	{
		for (int row : group)
		{
			for (int col = 0; col < 6; ++col)
			{
				if (model->item(row, col))
				{
					model->item(row, col)->setForeground(QBrush(QColor(Qt::red))); 
				}
			}
		}
	}

	tableView->setModel(model);
	tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableView->resizeRowsToContents();
}

QPair<vector<vector<double>>, vector<MatrixXd>> WBalance::correct_voltage(const MatrixXd &vol)
{
	//--25.3.4修改--用两个零载荷行计算的修正值，去修正它们之间所夹的行数据
	int rowCount = 144;
	int colCount = 6;
	//修正值
	int num = zerolines.size(); //零载荷行号组合数
	vector<vector<double>> zero_u_0;
	zero_u_0.clear();
	zero_u_0 = vector<vector<double>>(colCount, vector<double>(num, 0.0)); //6*num个修正值

	for (int i = 0; i < colCount; i++)
	{
		for (int j = 0; j < num; j++)
		{
			double sum = 0.0;
			for (int k : zerolines[j])
			{
				sum += vol(k, i);
			}
			zero_u_0[i][j] = sum / zerolines[j].size();
		}
	}

	//修正结果
	vector<MatrixXd> u_0_list;
	u_0_list.clear();
	int matNum = vol.cols();

	for (int i = 0; i < matNum; i++) //6列待修正电压
	{
		MatrixXd mat = vol.col(i);
		MatrixXd delta_u_0(rowCount, 1);    //144*1
		for (int j = 0; j < num; j++)       //每列有num个修正值，需要分成num组分别减去对应修正值
		{
			vector<int> zero = zerolines[j];
			for (int k = zero[0]; k <= zero[1]; k++)
			{
				delta_u_0(k, 0) = mat(k, 0) - zero_u_0[i][j];
			}
		}
		u_0_list.push_back(delta_u_0); //最后计算得到u_0_list包括6个144行的列向量
	}

	return QPair<vector<vector<double>>, vector<MatrixXd>>(zero_u_0, u_0_list); //返回修正值+修正结果
}

vector<MatrixXd> WBalance::compute_inde_variable(const vector<MatrixXd> &delta_u_i, const MatrixXd &f)
{
	int rowCount = delta_u_i[0].size();

	vector<MatrixXd> py_one_infer;//一阶干扰项

	for (int i = 0; i < f.cols(); i++)
	{
		MatrixXd a = f;
		a.col(i) = delta_u_i[i]; //用delta_u_0_list按顺序替换掉a中的一列
		py_one_infer.push_back(a); //得到6个144*6矩阵
	}

	MatrixXd py_two_infer(rowCount, 21); //二阶干扰项及交叉项
	for (int i = 0; i < 6; i++) //py_two_infer前6项（列）：f中每一列每个元素的平方
	{
		py_two_infer.col(i) = f.col(i).array().square();
	}

	int index = 6;
	for (int i = 0; i < 5; i++)//py_two_infer后15项（列）：第i列每个元素和第j（>i）列每个元素相乘
	{
		for (int j = i + 1; j < 6; j++)
		{
			py_two_infer.col(index++) = f.col(i).array() * f.col(j).array();
		}
	}

	vector<MatrixXd> Ai; //自变量矩阵
	for (int i = 0; i < 6; i++)
	{
		MatrixXd a(rowCount, py_one_infer[i].cols() + py_two_infer.cols()); //初始化144*27矩阵
		a << py_one_infer[i], py_two_infer; //拼接矩阵[py_one_infer[i],py_two_infer]
		Ai.push_back(a); //A：6个144*27矩阵
	}
	return Ai;
}

MatrixXd WBalance::compute_coefficient_Y(const vector<MatrixXd> &ai, const MatrixXd &f)
{
	vector<MatrixXd> X; //系数矩阵
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
		mtx.block(0, colIdx, rowCount, colCount) = mat; //遍历mtx中的每一个向量mat，并将其逐列拼接到mtx中，最后得到一个27*6的矩阵
		colIdx++;
	}

	return mtx;
}

void WBalance::save_calibra_result(const QString &Path)
{
	//1.保存系数矩阵
	QFile file_1(QDir(Path).filePath("系数矩阵.txt"));
	if (file_1.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out_1(&file_1);
		out_1.setCodec("UTF-8");
		//打印表头
		out_1 << QString("┌───────┬────────────────────────────────────┐\n");
		out_1 << QString("项目　＼　单元│           Y          Mz          Mx           X           Z          My│\n");
		out_1 << QString("├───────┼────────────────────────────────────┤\n");

		QStringList rowLabels = { "Y", "Mz", "Mx", "X", "Z", "My", "Y.Y", "Mz.Mz", "Mx.Mx", "X.X", "Z.Z", "My.My","Y.Mz",
			"Y.Mx", "Y.X", "Y.Z", "Y.My", "Mz.Mx", "Mz.X", "Mz.Z", "Mz.My","Mx.X", "Mx.Z", "Mx.My", "X.Z", "X.My", "Z.My" };

		//打印每一行数据
		for (int i = 0; i < coeff_average.rows(); ++i)
		{
			//打印行标签
			out_1 << QString("│ 　%1     │").arg(rowLabels[i], -6);
			for (int j = 0; j < coeff_average.cols(); ++j)
			{
				double value = coeff_average(i, j);
				out_1 << QString::asprintf("%12.4e", value);
				if (j < coeff_average.cols() - 1)
					out_1 << QString(" "); //列与列之间的间隔
			}
			out_1 << QString("│\n"); //行尾结束
		}

		//打印表格结束线
		out_1 << QString("└───────┴────────────────────────────────────┘\n");
		file_1.close();
	}
	else
	{
		QMessageBox::critical(this, tr("错误!"), tr("无法保存系数矩阵到文件！"));
	}

	//2.保存组桥结果
	/*QFile file_2(QDir(Path).filePath("组桥结果.txt"));
	if (file_2.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out_2(&file_2);
		out_2.setCodec("UTF-8");
		int row_2 = compute_u_0_list[0].rows(); //144
		int col_2 = compute_u_0_list.size(); //6
		for (int i = 0; i < row_2; ++i)
		{
			for (int j = 0; j < col_2; ++j)
			{
				double value = compute_u_0_list[j](i, 0);
				out_2 << QString::number(value, 'f', 12);
				if (j < col_2 - 1)
					out_2 << QString("  "); //列与列之间的间隔
			}
			out_2 << QString("\n"); //每行结束后换行
		}
		file_2.close();
	}
	else
	{
		QMessageBox::critical(this, tr("错误!"), tr("无法保存组桥结果到文件！"));
	}*/

	//3.保存修正值
	QFile file_3(QDir(Path).filePath("修正值.txt"));
	if (file_3.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out_3(&file_3);
		out_3.setCodec("UTF-8");
		int row_3 = zero_rows_u_0.size();      //6行
		int col_3 = zero_rows_u_0[0].size();   //10列
		for (int i = 0; i < col_3; ++i)
		{
			for (int j = 0; j < row_3; ++j)
			{
				double value = zero_rows_u_0[j][i];
				out_3 << QString::number(value, 'f', 12);
				if (j < row_3 - 1)
				{
					out_3 << QString("  ");
				}
			}
			out_3 << QString("\n");
		}
		file_3.close();
	}
	else
	{
		QMessageBox::critical(this, tr("错误!"), tr("无法保存修正值到文件！"));
	}

	//4.保存修正结果
	QFile file_4(QDir(Path).filePath("修正结果.txt"));
	if (file_4.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out_4(&file_4);
		out_4.setCodec("UTF-8");
		int row_4 = delta_u_0_list[0].rows(); //144
		int col_4 = delta_u_0_list.size(); //6
		for (int i = 0; i < row_4; ++i)
		{
			for (int j = 0; j < col_4; ++j)
			{
				double value = delta_u_0_list[j](i, 0);
				out_4 << QString::number(value, 'f', 12);
				if (j < col_4 - 1)
					out_4 << QString("  ");
			}
			out_4 << QString("\n");
		}
		file_4.close();
	}
	else
	{
		QMessageBox::critical(this, tr("错误!"), tr("无法保存修正结果到文件！"));
	}

	//5.保存自变量矩阵
	QFile file_5(QDir(Path).filePath("各分量对应的输入量.txt"));
	if (file_5.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out_5(&file_5);
		out_5.setCodec("UTF-8");
		int num_A = A_matrix.size();    //6个矩阵
		int row_5 = A_matrix[0].rows(); //144行
		int col_5 = A_matrix[0].cols(); //27列
		QStringList matrixTitles = { "Nx对应输入量", "Ny对应输入量", "Nz对应输入量", "Mx对应输入量", "My对应输入量", "Mz对应输入量" };
		for (int m = 0; m < num_A; ++m)
		{
			out_5 << matrixTitles[m] << QString("\n");

			//输出第m个矩阵的内容
			for (int i = 0; i < row_5; ++i)
			{
				for (int j = 0; j < col_5; ++j)
				{
					double value = A_matrix[m](i, j);
					out_5 << QString::number(value, 'f', 12);
					if (j < col_5 - 1)
					{
						out_5 << QString("  ");
					}
				}
				out_5 << QString("\n");
			}
			out_5 << QString("\n");
		}
		file_5.close();
	}
	else
	{
		QMessageBox::critical(this, tr("错误!"), tr("无法保存各分量对应的输入量到文件！"));
	}
}

QPair<vector<MatrixXd>, vector<MatrixXd>> WBalance::data_sample()
{
	int num = 100000; //抽样次数
	int row = 144;
	int col = 6;
	vector<MatrixXd> vol_results;
	vector<MatrixXd> load_results;
	random_device rd;
	mt19937 gen(rd());

	for (int k = 0; k < num; k++)
	{
		MatrixXd vol_temp_result = MatrixXd::Zero(row, col);
		MatrixXd load_temp_result = MatrixXd::Zero(row, col);
		for (int i = 0; i < row; i++)
		{
			//按行抽样
			for (int j = 0; j < col; j++)
			{
				if (f_matrixSD(i, j) == 0.0)
				{
					load_temp_result(i, j) = f_matrix(i, j);
				}
				else
				{
					normal_distribution<double> load_normal_dist(f_matrix(i, j), fabs(f_matrixSD(i, j))); //载荷抽样
					load_temp_result(i, j) = load_normal_dist(gen);
				}

				if (u_matrixSD(i, j) == 0.0)
				{
					vol_temp_result(i, j) = u_matrix(i, j);
				}
				else
				{
					normal_distribution<double> vol_normal_dist(u_matrix(i, j), fabs(u_matrixSD(i, j))); //电压抽样
					vol_temp_result(i, j) = vol_normal_dist(gen);
				}
			}
		}

		if ((k + 1) % 10000 == 0)
		{
			int value = (k + 1) / 10000;
			qDebug() << "value=" << value;
			ui.progressBar_coffee->setValue(value);
		}

		vol_results.push_back(vol_temp_result);
		load_results.push_back(load_temp_result);
	}
	qDebug() << "抽样完成";
	/*QString name = "D:/M组电压抽样数据.txt";
	QFile file(name);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "无法打开文件：" << name;
		return QPair<vector<MatrixXd>, vector<MatrixXd>>();
	}
	QTextStream out(&file);
	int n = 0;
	for (const auto &matrices : vol_results)
	{
		out << tr("组合 #") << ++n << ":\n";

		for (int i = 0; i < matrices.rows(); i++)
		{
			for (int j = 0; j < matrices.cols(); j++)
			{
				out << QString::number(matrices(i, j), 'f', 12) << " ";
			}
			out << "\n";
		}
		out << "\n";
	}
	file.close();
	qDebug() << "电压抽样结果已保存到：" << name;

	QString name2 = "G:/天平-代码调试/M组载荷抽样数据.txt";
	QFile file2(name2);
	if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "无法打开文件：" << name;
		return QPair<vector<MatrixXd>, vector<MatrixXd>>();
	}
	QTextStream out2(&file2);
	int m = 0;
	for (const auto& matrices : load_results)
	{
		for (int i = 0; i < matrices.rows(); i++)
		{
			for (int j = 0; j < matrices.cols(); j++)
			{
				out2 << QString::number(matrices(i, j), 'f', 12) << " ";
			}
			out2 << "\n";
		}
		out2 << "\n\n";
	}
	file2.close();
	qDebug() << "载荷抽样结果已保存到：" << name2;*/

	return QPair<vector<MatrixXd>, vector<MatrixXd>> (vol_results, load_results);
}

QPair<MatrixXd, MatrixXd> WBalance::cal_sample_results(const vector<MatrixXd> &vol, const vector<MatrixXd> &load)
{
	int M = vol.size();
	vector<MatrixXd> coeffs(M, MatrixXd::Zero(27, 6));
	MatrixXd vol_matri;
	MatrixXd load_matri;

	qDebug() << "cal_sample_results";
	for (int i = 0; i < M; i++)
	{
		vol_matri = vol[i];
		load_matri = load[i];

		auto m = correct_voltage(vol_matri);  //修正电压，返回修正值和修正结果
		zero_rows_u_0 = m.first;              //电压修正值
		delta_u_0_list = m.second;            //电压修正结果

		vector<MatrixXd> a_i = compute_inde_variable(delta_u_0_list, load_matri); //传入电压+载荷		
		MatrixXd coeff = compute_coefficient_Y(a_i, load_matri);//等精度计算
		coeffs[i] = coeff;

		if (i % 1149 == 0)
		{
			int value = i / 1149 + 10;
			qDebug() << "value=" << value;
			ui.progressBar_coffee->setValue(value);
		}
	}

	//QString name = "D:/M组系数.txt";
	//QFile file(name);
	//if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	//{
	//	qDebug() << "无法打开文件：" << name;
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
	//qDebug() << "系数已保存到：" << name;

	MatrixXd error;
	MatrixXd averg;
	error = averg = MatrixXd::Zero(27, 6);

	for (int i = 0; i < M; i++)
	{
		//M个系数矩阵
		MatrixXd mat = coeffs[i];
		averg += mat;
	}
	averg = averg / M; //平均值

	ui.progressBar_coffee->setValue(98);

	for (int i = 0; i < M; i++) //残差和
	{
		MatrixXd vi = (coeffs[i] - averg).array().square();
		error += vi;
	}

	//标准不确定度
	if (M <= 1)
	{
		qDebug() << "样本数小于1，无法计算标准不确定度";
		return QPair<MatrixXd, MatrixXd>();
	}
	for (int i = 0; i < error.rows(); i++)
	{
		for (int j = 0; j < error.cols(); j++)
		{
			error(i, j) = qSqrt(error(i, j) / (M - 1));
		}
	}

	ui.progressBar_coffee->setValue(99);

	return QPair<MatrixXd, MatrixXd>(averg, error);
}

void WBalance::SelectFile_glist()
{
	if (ui.lineEdit_b_name->text().isEmpty() || ui.lineEdit_b_instru->text().isEmpty() || ui.lineEdit_b_date->text().isEmpty() || ui.lineEdit_b_person->text().isEmpty())
	{
		QMessageBox::information(this, tr("提示"), tr("请输入完整校准信息!"));
		return;
	}

	glist_path = OpenFile(ui.lineEdit_glist_path, QString("加载表"));
	qDebug() << "glist_path = " << glist_path;
	
	if (glist_path.isEmpty())
	{
		glist_path = ui.lineEdit_glist_path->text().trimmed();
		qDebug() << "glist_path is empty, operation aborted.";
		return;
	}

	auto temp = find_zeroRows_index(glist_path);
	glist_flag = temp.first;
	if (glist_flag)
	{
		data_confirm_flag = false;
		zerolines = temp.second;
	}
}

void WBalance::SelectFile_load()
{
	if (!glist_flag)
	{
		QMessageBox::information(this, tr("提示"), tr("请先选择加载表!"));
		return;
	}

	load_path = OpenFile(ui.lineEdit_load_path, QString("载荷"));
	qDebug() << "load_path = " << load_path;

	if (load_path.isEmpty())
	{
		load_path = ui.lineEdit_load_path->text().trimmed();
		qDebug() << "load_path is empty, operation aborted.";
		return;
	}

	auto temp = read_file(load_path);
	load_flag = temp.ok;
	load_header = temp.headers;
	qDebug() << load_header;

	if (load_flag)
	{
		data_confirm_flag = false;
		f_matrix = temp.data;
		show_data(load_header, f_matrix, ui.tableView_load); //表格展示
		//QMessageBox::information(this, tr("提示"), tr("载荷文件读取成功"));
	}
}

void WBalance::SelectFile_voltage()
{
	if (!glist_flag)
	{
		QMessageBox::information(this, tr("提示"), tr("请先选择加载表!"));
		return;
	}

	voltage_path = OpenFile(ui.lineEdit_voltage_path, QString("电压"));
	qDebug() << "voltage_path = " << voltage_path;

	if (voltage_path.isEmpty())
	{
		voltage_path = ui.lineEdit_voltage_path->text().trimmed();
		qDebug() << "voltage_path is empty, operation aborted.";
		return;
	}

	auto temp = read_file(voltage_path);
	voltage_flag = temp.ok;
	voltage_header = temp.headers;
	qDebug() << voltage_header;

	if (voltage_flag)
	{
		data_confirm_flag = false;
		u_matrix = temp.data;
		show_data(voltage_header, u_matrix, ui.tableView_voltage); //表格展示
		//QMessageBox::information(this, tr("提示"), tr("电压文件读取成功"));
	}
}

void WBalance::read_data_cofirm()
{
	qDebug() << "glist_flag" << glist_flag << endl << "load_flag" << load_flag << endl << "voltage_flag" << voltage_flag << endl;
	if (!glist_flag || !load_flag || !voltage_flag)
	{
		QMessageBox::information(this, tr("提示"), tr("请先选择数据文件！"));
		return;
	}

	data_confirm_flag = true;
	f_matrix = read_tableview(ui.tableView_load);
	u_matrix = read_tableview(ui.tableView_voltage);
	QMessageBox::information(this, tr("提示"), tr("数据文件已确认"));

	show_data_analyse();
}

void WBalance::SelectFile_loadSD()
{
	loadSD_path = OpenFile(ui.lineEdit_load_std_path, QString("载荷标准差"));
	qDebug() << "loadSD_path = " << loadSD_path;

	if (loadSD_path.isEmpty())
	{
		qDebug() << "loadSD_path is empty, operation aborted.";
		return;
	}

	auto temp = read_file(loadSD_path);
	loadSD_flag = temp.ok;
	QStringList header;

	if (loadSD_flag)
	{
		sd_confirm_flag = false;
		header = temp.headers;
		qDebug() << header;

		f_matrixSD = temp.data;
		show_data(header, f_matrixSD, ui.tableView_load_std);

		//QMessageBox::information(this, tr("提示"), tr("载荷标准差文件读取成功"));
	}
}

void WBalance::SelectFile_voltageSD()
{
	voltageSD_path = OpenFile(ui.lineEdit_vol_std_path, QString("电压标准差"));
	qDebug() << "voltageSD_path = " << voltageSD_path;

	if (voltageSD_path.isEmpty())
	{
		qDebug() << "voltageSD_path is empty, operation aborted.";
		return;
	}

	auto temp = read_file(voltageSD_path);
	voltageSD_flag = temp.ok;
	QStringList header;

	if (voltageSD_flag)
	{
		sd_confirm_flag = false;
		header = temp.headers;
		qDebug() << header;
		u_matrixSD = temp.data;
		show_data(header, u_matrixSD, ui.tableView_vol_std);
		//QMessageBox::information(this, tr("提示"), tr("电压标准差文件读取成功"));
	}
}

void WBalance::read_sd_cofirm()
{
	qDebug() << "loadSD_flag" << loadSD_flag << endl << "voltageSD_flag" << voltageSD_flag;
	if (!loadSD_flag || !voltageSD_flag )
	{
		QMessageBox::information(this, tr("提示"), tr("请先选择标准差文件！"));
		return;
	}
	sd_confirm_flag = true;
	QMessageBox::information(this, tr("提示"), tr("标准差文件已确认"));

	show_oncalibration();
}

void WBalance::coeff_evaluate()
{	
	qDebug() << "data_confirm_flag" << data_confirm_flag << endl << "sd_confirm_flag" << sd_confirm_flag;
	if (!data_confirm_flag||!sd_confirm_flag)
	{
		QMessageBox::information(this, tr("提示"), tr("未确认文件！"));
		return;
	}

	//抽样计算-不去零行+等精度
	const auto &sample_temp = data_sample();

	qDebug() << "11111111111";

	//voltage_sample_result = sample_temp.first;
	//load_sample_result = sample_temp.second;

	qDebug() << "22222222222";

	const auto &results = cal_sample_results(sample_temp.first, sample_temp.second);
	coeff_average = results.first;
	coeff_error = results.second;

	//1. 生成主项（6个）
	QStringList SDheader = load_header;
	//2. 生成平方项（6个）
	for (const QString &h : load_header)
	{
		SDheader << h + "." + h;
	}
	//3. 生成两两组合项（15个）
	for (int i = 0; i < load_header.size(); ++i)
	{
		for (int j = i + 1; j < load_header.size(); ++j)
		{
			SDheader << load_header[i] + "." + load_header[j];
		}
	}

	QStandardItemModel *model_avrg = new QStandardItemModel(27, 6, this);
	model_avrg->setVerticalHeaderLabels(SDheader);
	model_avrg->setHorizontalHeaderLabels(load_header);

	for (int i = 0; i < coeff_average.rows(); i++)
	{
		for (int j = 0; j < coeff_average.cols(); j++)
		{
			QStandardItem *item = new QStandardItem(QString("%1").arg(coeff_average(i, j), 0, 'e', 4));
			model_avrg->setItem(i, j, item);
		}
	}
	ui.tableView_cofe_averg->setModel(model_avrg);
	
	QStandardItemModel *model_std = new QStandardItemModel(27, 6, this);
	model_std->setVerticalHeaderLabels(SDheader);
	model_std->setHorizontalHeaderLabels(load_header);

	for (int i = 0; i < coeff_error.rows(); i++)
	{
		for (int j = 0; j < coeff_error.cols(); j++)
		{
			QStandardItem *item = new QStandardItem(QString("%1").arg(coeff_error(i, j), 0, 'e', 4));
			model_std->setItem(i, j, item);
		}
	}
	ui.tableView_cofe_std->setModel(model_std);

	ui.progressBar_coffee->setValue(100);
	QMessageBox::information(this, tr("提示"), tr("计算完成"));
}

void WBalance::save_calibration()
{
	QFileInfo fileInfo(glist_path);
	QString initialDir = fileInfo.absolutePath();

	if (ui.tableView_cofe_averg->model() == nullptr || ui.tableView_cofe_std->model() == nullptr)
	{
		QMessageBox::critical(this, tr("错误！"), tr("当前无内容可保存！"));
		return;
	}

	//确保目录路径以斜杠结尾
	QDir saveDir(initialDir);
	if (!initialDir.endsWith('/'))
		initialDir += '/';

	//生成三个文件的完整路径
	const QString file1Path = saveDir.filePath("MCM方法-系数估计值标准不确定度.txt");
	const QString file2Path = saveDir.filePath("MCM方法-系数估计值.txt");

	const QString bName = ui.lineEdit_b_name->text().trimmed();
	const QString file3Path = saveDir.filePath("MCM方法 -" + bName + ".txt");

	//生成表头
	QStringList headers2;
	headers2.append(load_header);

	for (const QString& h : load_header)
	{
		headers2 << h + "." + h;
	}

	for (int i = 0; i < load_header.size(); ++i)
	{
		for (int j = i + 1; j < load_header.size(); ++j)
		{
			headers2 << load_header[i] + "." + load_header[j];
		}
	}

	//保存文件1：系数标准不确定度
	if (!save_coeff_file(file1Path, coeff_error, tr("MCM方法-系数估计值标准不确定度"), headers2))
	{
		QMessageBox::critical(this, tr("错误"), tr("MCM方法-系数估计值标准不确定度保存失败！"));
		return;
	}

	//保存文件2：系数估计值
	if (!save_coeff_file(file2Path, coeff_average, tr("MCM方法-系数估计值"), headers2))
	{
		QMessageBox::critical(this, tr("错误"), tr("MCM方法-系数估计值保存失败！"));
		return;
	}

	//保存文件3：以天平名称命名的系数估计值
	QFile file3(file3Path);
	if (file3.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file3);
		out.setCodec("UTF-8");
		out << "27   6\n";

		for (int i = 0; i < coeff_average.rows(); ++i)
		{
			for (int j = 0; j < coeff_average.cols(); ++j)
			{
				out << QString::asprintf("%12.4e", coeff_average(i, j));
				if (j < coeff_average.cols() - 1)
					out << " ";
			}
			out << "\n";
		}
		file3.close();
	}
	else
	{
		QMessageBox::critical(this, tr("错误"), bName + tr(" 保存失败！"));
		return;
	}
	
	ui.lineEdit_coff_path->setText(file3Path);
	ui.lineEdit_coff_path_2->setText(file3Path);
	ui.lineEdit_EV_save_path->setText(file3Path);

	QMessageBox::information(this, tr("提示"), tr("保存完成，文件目录为：")+ initialDir);
}

//新增的通用保存函数
bool WBalance::save_coeff_file(const QString& path, const MatrixXd& data, const QString& title, const QStringList& headers)
{
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << title << QString("\n");

	QString headerLine = "           ";
	for (int i = 0; i < load_header.size(); ++i)
	{
		headerLine += QString("%1").arg(load_header[i], (i == load_header.size() - 1) ? -4 : -12);
	}
	headerLine += QString("│");

	//输出表格框架
	out << QString("┌───────┬────────────────────────────────────┐\n")
		<< QString("项目　＼　单元│") << headerLine << QString("\n")
		<< QString("├───────┼────────────────────────────────────┤\n");

	for (int i = 0; i < data.rows(); ++i)
	{
		out << QString("│ 　%1     │").arg(headers.value(i, "Unknown").left(6));
		for (int j = 0; j < data.cols(); ++j)
		{
			out << QString::asprintf("%12.4e", data(i, j));
			if (j < data.cols() - 1) out << QString(" ");
		}
		out << QString("│\n");
	}

	out << QString("└───────┴────────────────────────────────────┘\n");
	file.close();

	return true;
}



void WBalance::show_choosefile()
{
    ui.stackedWidget->setCurrentIndex(0);
}

void WBalance::show_data_analyse()
{
	ui.lineEdit_b_type_2->setText(ui.comboBox_b_type->currentText());
	ui.lineEdit_b_name_2->setText(ui.lineEdit_b_name->text().trimmed());
	ui.lineEdit_b_instru_2->setText(ui.lineEdit_b_instru->text().trimmed());
	ui.lineEdit_b_date_2->setText(ui.lineEdit_b_date->text().trimmed());
	ui.lineEdit_b_person_2->setText(ui.lineEdit_b_person->text().trimmed());

	ui.stackedWidget->setCurrentIndex(1);
}

void WBalance::show_oncalibration()
{
	ui.lineEdit_b_type_3->setText(ui.comboBox_b_type->currentText());
	ui.lineEdit_b_name_3->setText(ui.lineEdit_b_name->text().trimmed());
	ui.lineEdit_b_instru_3->setText(ui.lineEdit_b_instru->text().trimmed());
	ui.lineEdit_b_date_3->setText(ui.lineEdit_b_date->text().trimmed());
	ui.lineEdit_b_person_3->setText(ui.lineEdit_b_person->text().trimmed());

    ui.stackedWidget->setCurrentIndex(2);
}

void WBalance::help()
{
	QString exeDir = QApplication::applicationDirPath();
	QString txtFilePath = exeDir + QDir::separator() + "help.txt";
	qDebug() << "路径" << txtFilePath;

	//使用QDesktopServices打开文件
	if (!QDesktopServices::openUrl(QUrl::fromLocalFile(txtFilePath))) 
	{
		qDebug() << "Failed to open help file:" << txtFilePath;
	}
	else 
	{
		qDebug() << "Help file opened successfully.";
	}
}

QString WBalance::read_data00()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"));
	if (!filePath.isEmpty())
	{
		ui.lineEdit_data00_path->setText(filePath);
	}
	else
	{
		QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
	}
	return filePath;
}

double WBalance::ThirdRule(double input)
{
	bool isNegative = input < 0.0;
	double absVal = std::fabs(input);

	//1) 先截断到 3 位小数
	double truncated = std::floor(absVal * 1000.0) / 1000.0;

	//2) 计算 value_2 = 原始值 - truncated
	double value_2 = absVal - truncated;

	//3) 固定 value_1 = 0.001
	const double value_1 = 0.001;

	//4) 计算 value_3 = value_1 / 3
	const double value_3 = value_1 / 3.0; // ≈ 0.0003333333

	//5) 决定是否进位
	double result = truncated;
	if (value_2 >= value_3) 
	{
		result += value_1;
	}

	//恢复符号
	if (isNegative) result = -result;
	return result;
}

void WBalance::show_data00()
{
	QString filePath_data00 = read_data00();
	if (!QFile::exists(filePath_data00))
	{
		QMessageBox::critical(this, tr("错误！"), tr("未找到文件路径！"));
		return;
	}

	auto temp = read_file(filePath_data00);
	bool flag = temp.ok;

	QStringList header = temp.headers;
	qDebug() << header;

	if (!flag)
	{
		qDebug() << "读取失败！";
		return;
	}

	MatrixXd data00 = temp.data;
	int rowCount = data00.rows();
	int colCount = data00.cols();

	QStandardItemModel* model = new QStandardItemModel(rowCount, colCount, this);
	model->setHorizontalHeaderLabels(header);

	for (int i = 0; i < rowCount; ++i)
	{
		for (int j = 0; j < colCount; ++j)
		{
			QString snValue = QString::number(data00(i, j), 'f', 8).replace(QRegularExpression("\\.?0+$"), "");
			QStandardItem* item = new QStandardItem(snValue);
			model->setItem(i, j, item);
		}
	}

	ui.tableView_show_data00->setModel(model);
	ui.tableView_show_data00->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableView_show_data00->resizeRowsToContents();

	//QMessageBox::information(this, tr("提示"), tr("data00文件读取成功"));
}

void WBalance::compute_repeat_result()
{
	string filePath_coff = ui.lineEdit_coff_path->text().toStdString();
	string filePath_data00 = ui.lineEdit_data00_path->text().toStdString();

	if (filePath_data00.empty() || filePath_coff.empty())
	{
		QMessageBox::warning(this, tr("输入错误"), tr("请确保文件路径已正确输入！"));
		return;
	}

	try {
		//数据处理函数_Result
		pair<map<int, vector<double>>, vector<map<int, vector<double>>>> result = _Result(filePath_data00, filePath_coff);
		MatrixXd iteration = map_to_mat(result.first);

		vector<string> tableHeader = readFirstLine(filePath_data00);
		QStringList header;
		for (const auto& v : tableHeader)
		{
			header.append(QString::fromStdString(v));
		}

		header.replaceInStrings(QRegularExpression("^U"), "");

		//创建并填充数据模型
		QStandardItemModel* model = new QStandardItemModel();
		model->setHorizontalHeaderLabels(header);

		//设置行号
		for (int i = 0; i < iteration.rows(); ++i)
		{
			model->setHeaderData(i, Qt::Vertical, i + 1);
		}

		//填充数据
		for (int row = 0; row < iteration.rows(); ++row)
		{
			for (int col = 0; col < iteration.cols(); ++col)
			{
				QStandardItem* item = new QStandardItem(QString::number(iteration(row, col)));
				model->setItem(row, col, item);
			}
		}

		ui.tableView_show_data00_result->setModel(model);
		ui.tableView_show_data00_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui.tableView_show_data00_result->resizeRowsToContents();

		//评估
		vector<Stats> cfx_result = cfx(filePath_data00, filePath_coff);
		if (cfx_result.empty())
		{
			QMessageBox::critical(this, tr("计算错误"), tr("评估结果为空，请检查输入数据！"));
			return;
		}

		QStandardItemModel* model_2 = new QStandardItemModel(2, header.size()); //2行：标准差与扩展不确定度
		model_2->setHorizontalHeaderLabels(header);

		//行标签：评估结果与扩展不确定度
		QStringList rowHeaders = { tr("综合加载重复性"), tr("扩展不确定度（k=2）") };
		model_2->setVerticalHeaderLabels(rowHeaders);

		for (int col = 0; col < header.size(); ++col) 
		{
			//三分之一准则
			double value = ThirdRule(cfx_result[col].stddev);

			QStandardItem* originalItem = new QStandardItem(QString::number(value));
			model_2->setItem(0, col, originalItem);

			QString value_2 = QString("%1").arg(2.0 * value, 0, 'f', 3); //k=2
			QStandardItem* uncertaintyItem = new QStandardItem(value_2);
			model_2->setItem(1, col, uncertaintyItem);
		}

		ui.tableView_show00_result->setModel(model_2);
		ui.tableView_show00_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui.tableView_show00_result->resizeRowsToContents();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(this, tr("错误"), QString::fromStdString(e.what()));
	}
}

void WBalance::save_comprehen_cfx()
{
	//迭代结果保存
	if (ui.tableView_show_data00_result->model() == nullptr || ui.tableView_show00_result->model() == nullptr)
	{
		QMessageBox::critical(this, tr("错误！"), tr("当前无内容可保存！"));
		return;
	}

	QAbstractItemModel* model = ui.tableView_show_data00_result->model();
	int rowCount = model->rowCount();
	int colCount = model->columnCount();

	QStringList horizontalHeaders;//水平表头
	for (int col = 0; col < colCount; ++col)
	{
		QVariant header = model->headerData(col, Qt::Horizontal);
		horizontalHeaders.append(header.toString());
	}

	QStringList verticalHeaders;//竖直表头
	for (int row = 0; row < rowCount; ++row)
	{
		QVariant header = model->headerData(row, Qt::Vertical);
		verticalHeaders.append(header.toString());
	}

	QString content;
	content += "\t" + horizontalHeaders.join("\t") + "\n"; // 添加水平表头

	for (int row = 0; row < rowCount; ++row)
	{
		content += verticalHeaders[row] + "\t"; // 添加垂直表头
		for (int col = 0; col < colCount; ++col)
		{
			QModelIndex index = model->index(row, col);
			content += model->data(index).toString() + "\t";
		}
		content += "\n";
	}

	QString filePath = ui.lineEdit_coff_path->text();
	QFileInfo fileInfo(filePath);
	QString initialDir = fileInfo.absolutePath();

	//确保目录路径以斜杠结尾
	QDir saveDir(initialDir);
	if (!initialDir.endsWith('/'))
		initialDir += '/';

	//生成2个文件的完整路径
	const QString file1Path = saveDir.filePath("综合加载重复性计算输出分量.txt");
	const QString file2Path = saveDir.filePath("综合加载重复性评估结果.txt");

	QFile file(file1Path);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << content;
		file.close();
		QMessageBox::information(this, tr("提示"), tr("迭代结果文件保存成功"));
	}
	else
	{
		QMessageBox::critical(this, tr("错误"), tr("迭代结果文件保存失败"));
	}

	//综合加载重复性评估结果保存
	QAbstractItemModel *model_show00 = ui.tableView_show00_result->model();

	int rowCount_show00 = model_show00->rowCount();
	int colCount_show00 = model_show00->columnCount();

	QStringList horizontalHeaders_show00;
	for (int col = 0; col < colCount_show00; ++col)
	{
		horizontalHeaders_show00.append(model_show00->headerData(col, Qt::Horizontal).toString());
	}

	QStringList verticalHeaders_show00;
	for (int row = 0; row < rowCount_show00; ++row)
	{
		verticalHeaders_show00.append(model_show00->headerData(row, Qt::Vertical).toString());
	}

	QString content_show00;
	content_show00 += "\t" + horizontalHeaders_show00.join("\t") + "\n"; //添加水平表头

	for (int row = 0; row < rowCount_show00; ++row)
	{
		content_show00 += verticalHeaders_show00[row] + "\t"; //添加垂直表头
		for (int col = 0; col < colCount_show00; ++col)
		{
			QModelIndex index = model_show00->index(row, col);
			content_show00 += model_show00->data(index).toString() + "\t";
		}
		content_show00 += "\n";
	}

	QFile file_show00(file2Path);
	if (file_show00.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file_show00);
		out << content_show00;
		file_show00.close();
		QMessageBox::information(this, tr("成功"), tr("评估结果文件保存成功"));
	}
	else
	{
		QMessageBox::critical(this, tr("错误"), tr("保存失败"));
	}
}

void WBalance::read_load01_data()
{
	if (ui.lineEdit_load_path->text().isEmpty())
	{
		if (!ui.lineEdit_coff_path_2->text().isEmpty())
		{
			QFileInfo fileInfo(ui.lineEdit_coff_path_2->text());
			QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), fileInfo.absolutePath());
			if (!filePath.isEmpty())
			{
				ui.lineEdit_load01_path->setText(filePath);
				string filePath_load01 = ui.lineEdit_load01_path->text().toStdString();
				//string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();
				if (!QFile::exists(QString::fromStdString(filePath_load01)))
				{
					QMessageBox::critical(this, tr("错误！"), tr("路径不存在！"));
					return;
				}
				vector<string> tableHeader = readFirstLine(filePath_load01);
				QStringList header;
				for (const auto& v : tableHeader)
				{
					header.append(QString::fromStdString(v));
				}

				QStandardItemModel* model = new QStandardItemModel();
				model->setHorizontalHeaderLabels(header);

				auto data = read_all_data(filePath_load01);
				MatrixXd data_load = map_to_mat(data);
				for (int i = 0; i < data_load.rows(); i++)
				{
					model->setHeaderData(i, Qt::Vertical, i + 1);
				}

				for (int row = 0; row < data_load.rows(); ++row)
				{
					for (int col = 0; col < data_load.cols(); ++col)
					{
						QStandardItem* item = new QStandardItem(QString::number(data_load(row, col)));
						model->setItem(row, col, item);
					}
				}
				ui.tableView_show_load01->setModel(model);
				ui.tableView_show_load01->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				ui.tableView_show_load01->resizeRowsToContents();
			}
			else
			{
				QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
			}
		}
		else
		{
			QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath());
			if (!filePath.isEmpty())
			{
				ui.lineEdit_load01_path->setText(filePath);
				ui.lineEdit_load01_path->setText(filePath);
				string filePath_load01 = ui.lineEdit_load01_path->text().toStdString();
				//string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();
				if (!QFile::exists(QString::fromStdString(filePath_load01)))
				{
					QMessageBox::critical(this, tr("错误！"), tr("路径不存在！"));
					return;
				}
				vector<string> tableHeader = readFirstLine(filePath_load01);
				QStringList header;
				for (const auto& v : tableHeader)
				{
					header.append(QString::fromStdString(v));
				}

				QStandardItemModel* model = new QStandardItemModel();
				model->setHorizontalHeaderLabels(header);

				auto data = read_all_data(filePath_load01);
				MatrixXd data_load = map_to_mat(data);
				for (int i = 0; i < data_load.rows(); i++)
				{
					model->setHeaderData(i, Qt::Vertical, i + 1);
				}

				for (int row = 0; row < data_load.rows(); ++row)
				{
					for (int col = 0; col < data_load.cols(); ++col)
					{
						QStandardItem* item = new QStandardItem(QString::number(data_load(row, col)));
						model->setItem(row, col, item);
					}
				}
				ui.tableView_show_load01->setModel(model);
				ui.tableView_show_load01->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				ui.tableView_show_load01->resizeRowsToContents();
			}
			else
			{
				QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
			}
		}

	}
	else
	{
		QFileInfo fileInfo(ui.lineEdit_load_path->text());
		QString directory = fileInfo.absolutePath();
		QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), directory);
		if (!filePath.isEmpty())
		{
			ui.lineEdit_load01_path->setText(filePath);
			ui.lineEdit_load01_path->setText(filePath);
			string filePath_load01 = ui.lineEdit_load01_path->text().toStdString();
			//string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();
			if (!QFile::exists(QString::fromStdString(filePath_load01)))
			{
				QMessageBox::critical(this, tr("错误！"), tr("路径不存在！"));
				return;
			}
			vector<string> tableHeader = readFirstLine(filePath_load01);
			QStringList header;
			for (const auto& v : tableHeader)
			{
				header.append(QString::fromStdString(v));
			}

			QStandardItemModel* model = new QStandardItemModel();
			model->setHorizontalHeaderLabels(header);

			auto data = read_all_data(filePath_load01);
			MatrixXd data_load = map_to_mat(data);
			for (int i = 0; i < data_load.rows(); i++)
			{
				model->setHeaderData(i, Qt::Vertical, i + 1);
			}

			for (int row = 0; row < data_load.rows(); ++row)
			{
				for (int col = 0; col < data_load.cols(); ++col)
				{
					QStandardItem* item = new QStandardItem(QString::number(data_load(row, col)));
					model->setItem(row, col, item);
				}
			}
			ui.tableView_show_load01->setModel(model);
			ui.tableView_show_load01->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
			ui.tableView_show_load01->resizeRowsToContents();
		}
		else
		{
			QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
		}
	}

}

void WBalance::read_data01_data()
{
	if (ui.lineEdit_load_path->text().isEmpty())
	{
		if (!ui.lineEdit_coff_path_2->text().isEmpty())
		{
			QFileInfo fileInfo(ui.lineEdit_coff_path_2->text());
			QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), fileInfo.absolutePath());
			if (!filePath.isEmpty())
			{
				ui.lineEdit_data01_path_2->setText(filePath);
				//string filePath_load01 = ui.lineEdit_load01_path->text().toStdString();
				string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();

				vector<string> tableHeader1 = readFirstLine(filePath_data01);
				QStringList header1;
				for (const auto& v : tableHeader1)
				{
					header1.append(QString::fromStdString(v));
				}

				QStandardItemModel* model1 = new QStandardItemModel();
				model1->setHorizontalHeaderLabels(header1);

				auto data1 = read_all_data(filePath_data01);
				MatrixXd data_load1 = map_to_mat(data1);
				for (int i = 0; i < data_load1.rows(); i++)
				{
					model1->setHeaderData(i, Qt::Vertical, i + 1);
				}

				for (int row = 0; row < data_load1.rows(); ++row)
				{
					for (int col = 0; col < data_load1.cols(); ++col)
					{
						QStandardItem* item1 = new QStandardItem(QString::number(data_load1(row, col)));
						model1->setItem(row, col, item1);
					}
				}
				ui.tableView_show_data01->setModel(model1);
				ui.tableView_show_data01->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				ui.tableView_show_data01->resizeRowsToContents();
			}
			else
			{
				QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
			}
		}
		else
		{
			QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath());
			if (!filePath.isEmpty())
			{
				ui.lineEdit_data01_path_2->setText(filePath);
				string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();

				vector<string> tableHeader1 = readFirstLine(filePath_data01);
				QStringList header1;
				for (const auto& v : tableHeader1)
				{
					header1.append(QString::fromStdString(v));
				}

				QStandardItemModel* model1 = new QStandardItemModel();
				model1->setHorizontalHeaderLabels(header1);

				auto data1 = read_all_data(filePath_data01);
				MatrixXd data_load1 = map_to_mat(data1);
				for (int i = 0; i < data_load1.rows(); i++)
				{
					model1->setHeaderData(i, Qt::Vertical, i + 1);
				}

				for (int row = 0; row < data_load1.rows(); ++row)
				{
					for (int col = 0; col < data_load1.cols(); ++col)
					{
						QStandardItem* item1 = new QStandardItem(QString::number(data_load1(row, col)));
						model1->setItem(row, col, item1);
					}
				}
				ui.tableView_show_data01->setModel(model1);
				ui.tableView_show_data01->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				ui.tableView_show_data01->resizeRowsToContents();
			}
			else
			{
				QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
			}
		}
	}
	else
	{
		QFileInfo fileInfo(ui.lineEdit_load_path->text());
		QString directory = fileInfo.absolutePath();
		QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), directory);
		if (!filePath.isEmpty())
		{
			ui.lineEdit_data01_path_2->setText(filePath);
			string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();

			vector<string> tableHeader1 = readFirstLine(filePath_data01);
			QStringList header1;
			for (const auto& v : tableHeader1)
			{
				header1.append(QString::fromStdString(v));
			}

			QStandardItemModel* model1 = new QStandardItemModel();
			model1->setHorizontalHeaderLabels(header1);

			auto data1 = read_all_data(filePath_data01);
			MatrixXd data_load1 = map_to_mat(data1);
			for (int i = 0; i < data_load1.rows(); i++)
			{
				model1->setHeaderData(i, Qt::Vertical, i + 1);
			}

			for (int row = 0; row < data_load1.rows(); ++row)
			{
				for (int col = 0; col < data_load1.cols(); ++col)
				{
					QStandardItem* item1 = new QStandardItem(QString::number(data_load1(row, col)));
					model1->setItem(row, col, item1);
				}
			}
			ui.tableView_show_data01->setModel(model1);
			ui.tableView_show_data01->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
			ui.tableView_show_data01->resizeRowsToContents();
		}
		else
		{
			QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
		}
	}
}


void WBalance::show_data01_load01()
{
	string filePath_load01 = ui.lineEdit_load01_path->text().toStdString();
	string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();
	if (!QFile::exists(QString::fromStdString(filePath_load01)) || !QFile::exists(QString::fromStdString(filePath_data01)))
	{
		QMessageBox::critical(this, tr("错误！"), tr("必须读取两个数据文件！"));
		return;
	}
	else
	{
		ui.stackedWidget->setCurrentIndex(6);
		ui.lineEdit_b_type_4->setText(ui.comboBox_b_type->currentText());
		ui.lineEdit_b_name_5->setText(ui.lineEdit_b_name->text().trimmed());
		ui.lineEdit_b_instru_5->setText(ui.lineEdit_b_instru->text().trimmed());
		ui.lineEdit_b_date_5->setText(ui.lineEdit_b_date->text().trimmed());
		ui.lineEdit_b_person_5->setText(ui.lineEdit_b_person->text().trimmed());

	}
}

void WBalance::compute_comprehen()
{
	if (ui.lineEdit_coff_path_2->text().isEmpty() || ui.lineEdit_load01_path->text().isEmpty() || ui.lineEdit_data01_path_2->text().isEmpty())
	{
		QMessageBox::critical(this, tr("错误！"), tr("数据不足，无法计算"));
		return;
	}

	bool isChecked_method1 = ui.checkBox_method1->isChecked();
	bool isChecked_method2 = ui.checkBox_method1_2->isChecked();

	if (isChecked_method1 == isChecked_method2)
	{
		QMessageBox::critical(this, tr("错误！"), tr("请选择一种综合加载误差评估方法！"));
		return;
	}

	string filePath_load01 = ui.lineEdit_load01_path->text().toStdString();
	string filePath_data01 = ui.lineEdit_data01_path_2->text().toStdString();
	string filePath_coff = ui.lineEdit_coff_path_2->text().toStdString();

	pair<map<int, vector<double>>, vector<map<int, vector<double>>>> result = _Result(filePath_data01, filePath_coff);
	MatrixXd iteration = map_to_mat(result.first);

	//先设置表头
	vector<string> tableHeader = readFirstLine(filePath_load01);
	QStringList header;
	for (const auto& v : tableHeader)
	{
		header.append(QString::fromStdString(v));
	}

	QStandardItemModel* model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(header);

	for (int i = 0; i < iteration.rows(); i++)
	{
		model->setHeaderData(i, Qt::Vertical, i + 1);
	}

	for (int row = 0; row < iteration.rows(); ++row)
	{
		for (int col = 0; col < iteration.cols(); ++col)
		{
			QStandardItem* item = new QStandardItem(QString::number(iteration(row, col)));
			model->setItem(row, col, item);
		}
	}
	ui.tableView_iteration_out->setModel(model);
	ui.tableView_iteration_out->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableView_iteration_out->resizeRowsToContents();

	if (isChecked_method1 == 1)
	{
		vector<double> method1_result = A2244(filePath_data01, filePath_load01, filePath_coff);
		if (method1_result.empty())
		{
			qDebug() << "method1_result is empty!";
			return;
		}
		QStandardItemModel* model1 = new QStandardItemModel();
		model1->setHorizontalHeaderLabels(header);
		QStringList method1_rowHeaders = { tr("综合加载误差"),tr("综合加载误差引入的各分量扩展不确定度（k=2）") };
		model1->setVerticalHeaderLabels(method1_rowHeaders);
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < method1_result.size(); j++)
			{
				if (i == 0)
				{
					QStandardItem* item1 = new QStandardItem(QString::number(method1_result[j]));
					model1->setItem(i, j, item1);
				}
				else if (i == 1)
				{
					QStandardItem* item1 = new QStandardItem(QString::number(2 * method1_result[j]));
					model1->setItem(i, j, item1);
				}
			}
		}
		ui.tableView_comprehen_err_ass->setModel(model1);
		ui.tableView_comprehen_err_ass->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui.tableView_comprehen_err_ass->resizeRowsToContents();
	}
	else if (isChecked_method2 == 1)
	{
		MatrixXd result_66 = compute_66_matrix(filePath_data01, filePath_load01, filePath_coff);

		//表头加"K_"
		QStringList header1 = header;
		header1.replaceInStrings(QRegularExpression("^"), "K_");
		QStandardItemModel* model2 = new QStandardItemModel();
		model2->setHorizontalHeaderLabels(header1);

		//加"Δ"
		QStringList header2 = header;
		header2.replaceInStrings(QRegularExpression("^"), ("Δ"));
		model2->setVerticalHeaderLabels(header2);

		for (int row = 0; row < result_66.rows(); ++row)
		{
			for (int col = 0; col < result_66.cols(); ++col)
			{
				QStandardItem* item2 = new QStandardItem(QString::number(result_66(row, col)));
				model2->setItem(row, col, item2);
			}
		}
		ui.tableView_comprehen_66->setModel(model2);
		ui.tableView_comprehen_66->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui.tableView_comprehen_66->resizeRowsToContents();
	}
}

void WBalance::save_comprehen()
{
	if (ui.checkBox_method1->isChecked() == ui.checkBox_method1_2->isChecked())
	{
		QMessageBox::critical(this, tr("错误！"), tr("请选择一种综合加载误差评估方法！"));
		return;
	}

	if (ui.tableView_iteration_out->model() == nullptr || ui.tableView_comprehen_err_ass->model() == nullptr && ui.tableView_comprehen_66->model() == nullptr)
	{
		QMessageBox::critical(this, tr("错误！"), tr("当前无内容可保存！"));
		return;
	}

	QString filePath = ui.lineEdit_load_path->text();
	if (filePath.isEmpty())//如果前面没有路径，就重新选择路径（独立使用时）
	{
		QFileInfo fileInfo(ui.lineEdit_coff_path_2->text());
		QString saveFilePath = QFileDialog::getSaveFileName(this, tr("选择文件"), fileInfo.absolutePath(), ("*.txt"));
		if (saveFilePath.isEmpty())
		{
			QMessageBox::critical(this, tr("错误！"), tr("请选择保存路径！"));
			return;
		}
		else
		{
			QAbstractItemModel* model = ui.tableView_iteration_out->model();
			int rowCount = model->rowCount();
			int colCount = model->columnCount();
			QStringList horizontalHeaders;//水平表头
			for (int col = 0; col < colCount; ++col)
			{
				QVariant header = model->headerData(col, Qt::Horizontal);
				horizontalHeaders.append(header.toString());
			}

			QStringList verticalHeaders;//竖直表头
			for (int row = 0; row < rowCount; ++row)
			{
				QVariant header = model->headerData(row, Qt::Vertical);
				verticalHeaders.append(header.toString());
			}

			QString content;
			content += "\t" + horizontalHeaders.join("\t\t") + "\n"; // 添加水平表头

			for (int row = 0; row < rowCount; ++row)
			{
				content += verticalHeaders[row] + "\t"; // 添加垂直表头
				for (int col = 0; col < colCount; ++col) 
				{
					QModelIndex index = model->index(row, col);
					content += model->data(index).toString() + "\t";
				}
				content += "\n";
			}

			QFile file(saveFilePath);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream out(&file);
				out << content;
				file.close();
				QMessageBox::information(this, tr("提示"), tr("迭代结果文件保存成功"));
			}
			else
			{
				QMessageBox::critical(this, tr("错误"), tr("迭代结果文件保存失败"));
			}

			//法一，法二的保存
			if (ui.checkBox_method1->isChecked() && !ui.checkBox_method1_2->isChecked())
			{
				qDebug() << "1111111111";
				QFileInfo fileInfo(saveFilePath);
				QString directory = fileInfo.absolutePath() + "/" + tr("综合加载误差评估方法一.txt");

				QAbstractItemModel* model1 = ui.tableView_comprehen_err_ass->model();
				int rowCount = model1->rowCount();
				int colCount = model1->columnCount();
				QStringList horizontalHeaders1;//水平表头
				for (int col = 0; col < colCount; ++col)
				{
					QVariant header = model1->headerData(col, Qt::Horizontal);
					horizontalHeaders1.append(header.toString());
				}

				QStringList verticalHeaders1;//竖直表头
				for (int row = 0; row < rowCount; ++row)
				{
					QVariant header = model1->headerData(row, Qt::Vertical);
					verticalHeaders1.append(header.toString());
				}

				QString content1;
				content1 += "\t" + horizontalHeaders1.join("\t\t") + "\n"; // 添加水平表头

				for (int row = 0; row < rowCount; ++row)
				{
					content1 += verticalHeaders1[row] + "\t"; // 添加垂直表头
					for (int col = 0; col < colCount; ++col) 
					{
						QModelIndex index = model1->index(row, col);
						content1 += model1->data(index).toString() + "\t";
					}
					content1 += "\n";
				}

				QFile file(directory);
				if (file.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					QTextStream out(&file);
					out << content1;
					file.close();
					QMessageBox::information(this, tr("提示"), tr("综合加载误差评估方法一文件保存成功"));
					ui.lineEdit_CLE_save_path->setText(directory);
				}
				else
				{
					QMessageBox::critical(this, tr("错误"), tr("综合加载误差评估方法一文件保存失败"));
				}
			}
			else if (ui.checkBox_method1_2->isChecked() && !ui.checkBox_method1->isChecked())
			{
				qDebug() << "222222222";
				QFileInfo fileInfo(saveFilePath);
				QString directory = fileInfo.absolutePath() + "/" + tr("综合加载误差评估方法二.txt");

				QAbstractItemModel* model2 = ui.tableView_comprehen_66->model();
				int rowCount = model2->rowCount();
				int colCount = model2->columnCount();
				QStringList horizontalHeaders2;//水平表头
				for (int col = 0; col < colCount; ++col)
				{
					QVariant header = model2->headerData(col, Qt::Horizontal);
					horizontalHeaders2.append(header.toString());
				}

				QStringList verticalHeaders2;//竖直表头
				for (int row = 0; row < rowCount; ++row)
				{
					QVariant header = model2->headerData(row, Qt::Vertical);
					verticalHeaders2.append(header.toString());
				}

				QString content2;
				content2 += "\t" + horizontalHeaders2.join("\t\t") + "\n"; // 添加水平表头

				for (int row = 0; row < rowCount; ++row)
				{
					content2 += verticalHeaders2[row] + "\t"; // 添加垂直表头
					for (int col = 0; col < colCount; ++col) {
						QModelIndex index = model2->index(row, col);
						content2 += model2->data(index).toString() + "\t";
					}
					content2 += "\n";
				}

				QFile file(directory);
				if (file.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					QTextStream out(&file);
					out << content2;
					file.close();
					QMessageBox::information(this, tr("提示"), tr("综合加载误差评估方法二文件保存成功"));
					ui.lineEdit_CLE_save_path->setText(directory);
				}
				else
				{
					QMessageBox::critical(this, tr("错误"), tr("综合加载误差评估方法二文件保存失败"));
				}
			}
		}
	}
	else//会将计算系数时读取文件的路径作为默认保存路径
	{
		QFileInfo fileInfo(filePath);
		QString directory = fileInfo.absolutePath();
		QString saveFilePath = QFileDialog::getSaveFileName(this, tr("保存文件"), directory, ("*.txt"));
		if (saveFilePath.isEmpty())
		{
			QMessageBox::critical(this, tr("错误！"), tr("未找到文件路径！"));
			return;
		}
		else
		{
			QAbstractItemModel* model = ui.tableView_iteration_out->model();
			int rowCount = model->rowCount();
			int colCount = model->columnCount();
			QStringList horizontalHeaders;//水平表头
			for (int col = 0; col < colCount; ++col)
			{
				QVariant header = model->headerData(col, Qt::Horizontal);
				horizontalHeaders.append(header.toString());
			}

			QStringList verticalHeaders;//竖直表头
			for (int row = 0; row < rowCount; ++row)
			{
				QVariant header = model->headerData(row, Qt::Vertical);
				verticalHeaders.append(header.toString());
			}

			QString content;
			content += "\t" + horizontalHeaders.join("\t\t") + "\n"; // 添加水平表头

			for (int row = 0; row < rowCount; ++row)
			{
				content += verticalHeaders[row] + "\t"; // 添加垂直表头
				for (int col = 0; col < colCount; ++col) {
					QModelIndex index = model->index(row, col);
					content += model->data(index).toString() + "\t";
				}
				content += "\n";
			}

			QFile file(saveFilePath);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream out(&file);
				out << content;
				file.close();
				QMessageBox::information(this, tr("提示"), tr("迭代结果文件保存成功"));
			}
			else
			{
				QMessageBox::critical(this, tr("错误"), tr("迭代结果文件保存失败"));
			}

			//法一，法二的保存
			if (ui.checkBox_method1->isChecked() && !ui.checkBox_method1_2->isChecked())
			{

				QString directory_1 = directory + "/" + tr("综合加载误差评估方法一.txt");

				QAbstractItemModel* model1 = ui.tableView_comprehen_err_ass->model();
				int rowCount = model1->rowCount();
				int colCount = model1->columnCount();
				QStringList horizontalHeaders1;//水平表头
				for (int col = 0; col < colCount; ++col)
				{
					QVariant header = model1->headerData(col, Qt::Horizontal);
					horizontalHeaders1.append(header.toString());
				}

				QStringList verticalHeaders1;//竖直表头
				for (int row = 0; row < rowCount; ++row)
				{
					QVariant header = model1->headerData(row, Qt::Vertical);
					verticalHeaders1.append(header.toString());
				}

				QString content1;
				content1 += "\t" + horizontalHeaders1.join("\t\t") + "\n"; // 添加水平表头

				for (int row = 0; row < rowCount; ++row)
				{
					content1 += verticalHeaders1[row] + "\t"; // 添加垂直表头
					for (int col = 0; col < colCount; ++col) {
						QModelIndex index = model1->index(row, col);
						content1 += model1->data(index).toString() + "\t";
					}
					content1 += "\n";
				}

				QFile file(directory_1);
				if (file.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					QTextStream out(&file);
					out << content1;
					file.close();
					QMessageBox::information(this, tr("提示"), tr("综合加载误差评估方法一文件保存成功"));
					ui.lineEdit_CLE_save_path->setText(directory_1);
				}
				else
				{
					QMessageBox::critical(this, tr("错误"), tr("综合加载误差评估方法一文件保存失败"));
				}
			}
			else if (ui.checkBox_method1_2->isChecked() && !ui.checkBox_method1->isChecked())
			{

				QString directory_2 = directory + "/" + tr("综合加载误差评估方法二.txt");

				QAbstractItemModel* model2 = ui.tableView_comprehen_66->model();
				int rowCount = model2->rowCount();
				int colCount = model2->columnCount();
				QStringList horizontalHeaders2;//水平表头
				for (int col = 0; col < colCount; ++col)
				{
					QVariant header = model2->headerData(col, Qt::Horizontal);
					horizontalHeaders2.append(header.toString());
				}

				QStringList verticalHeaders2;//竖直表头
				for (int row = 0; row < rowCount; ++row)
				{
					QVariant header = model2->headerData(row, Qt::Vertical);
					verticalHeaders2.append(header.toString());
				}

				QString content2;
				content2 += "\t" + horizontalHeaders2.join("\t\t") + "\n"; // 添加水平表头

				for (int row = 0; row < rowCount; ++row)
				{
					content2 += verticalHeaders2[row] + "\t"; // 添加垂直表头
					for (int col = 0; col < colCount; ++col) {
						QModelIndex index = model2->index(row, col);
						content2 += model2->data(index).toString() + "\t";
					}
					content2 += "\n";
				}

				QFile file(directory_2);
				if (file.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					QTextStream out(&file);
					out << content2;
					file.close();
					QMessageBox::information(this, tr("提示"), tr("综合加载误差评估方法二文件保存成功"));
					ui.lineEdit_CLE_save_path->setText(directory_2);
				}
				else
				{
					QMessageBox::critical(this, tr("错误"), tr("综合加载误差评估方法二文件保存失败"));
				}
			}
			else if (ui.checkBox_method1->isChecked() == ui.checkBox_method1_2->isChecked())
			{
				QMessageBox::critical(this, tr("错误！"), tr("请选择一种综合加载误差评估方法！"));
				return;
			}
		}
	}
}


void WBalance::handleFileSelection(QLineEdit* lineEdit)
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath());
	if (!filePath.isEmpty()) {
		lineEdit->setText(filePath);
	}
	else {
		QMessageBox::information(this, tr("提示"), tr("请重新选择文件！"));
	}
};

void WBalance::btnSaveClicked()
{
	ui.progressBar->setVisible(true);
	ui.progressBar->setValue(0);
	//在输出报告界面，如果用户修改有关内容，将以修改后的内容为准。
	QString  type_last = ui.lineEdit_b_type_8->text();
	QString  name_last = ui.lineEdit_b_name_8->text();
	QString  instru_last = ui.lineEdit_b_instru_8->text();
	QString  date_last = ui.lineEdit_b_date_8->text();
	QString  person_last = ui.lineEdit_b_person_8->text();

	QString  date_NP = date_last.remove('.');
	QString b_number_8 = name_last + "-" + date_NP;
	ui.lineEdit_b_number_8->setText(b_number_8);


	QString templatePath = "";
	if (ui.checkBox_method1->isChecked())
	{
		templatePath = "G:/Wind_tunnel_balance/ALL/输出报告-设计稿-方案1.dotx";
	}
	if (ui.checkBox_method1_2->isChecked())
	{
		templatePath = "G:/Wind_tunnel_balance/ALL/输出报告-设计稿-方案2.dotx";
	}
	qDebug() << templatePath;
	if (templatePath.isEmpty())
	{
		QMessageBox::critical(this, "错误", "模板文件路径为空，请检查输入！");
		ui.progressBar->setVisible(false);
		return;
	}


	QString savePath = "";
	if (!glist_path.isEmpty()) 
	{
		QFileInfo fileInfo(glist_path);
		QString directory1 = fileInfo.absolutePath();
		savePath = directory1 + "/" + "输出报告结果" + b_number_8 + ".docx";
	}
	else
	{
		QFileInfo fileInfo(templatePath);
		QString directory2 = fileInfo.absolutePath();
		savePath = directory2 + "/" + "输出报告结果" + b_number_8 + ".docx";
	}
	qDebug() << savePath;

	QFile savefile(savePath);

	if (savefile.exists())
	{
		if (!savefile.open(QIODevice::ReadWrite))
		{
			QString errorMessage = QString("无法打开文件 %1，可能文件已被占用。请选择新的保存路径。").arg(savePath);
			QMessageBox::warning(this, "警告", errorMessage);
			QString newSavePath = QFileDialog::getSaveFileName(this, "文件已被占用，请另存为", "", "Word Documents (*.docx)");
			if (newSavePath.isEmpty())
			{
				QMessageBox::warning(this, "警告", "未选择新的保存路径，操作取消");
				return;
			}
			savePath = newSavePath;
		}
		savefile.close();
	}
	// 读取文件数据
	QStringList data1;
	for (int row = 0; row < 2; ++row)
	{
		for (int col = 0; col < 6; ++col)
		{
			QTableWidgetItem* item = ui.TableWidget_b->item(row, col);
			data1 << (item ? item->text() : "");
		}
	}
	QStringList data2 = readTextFile(ui.lineEdit_EV_save_path->text());
	if (data2.isEmpty())
	{
		ui.progressBar->setVisible(false);
		return;
	}
	QStringList data3 = readTextFile(ui.lineEdit_SUEV_save_path->text());
	if (data3.isEmpty())
	{
		ui.progressBar->setVisible(false);
		return;
	}
	/*QStringList data4 = readTextFile(ui.lineEdit_UCLR_save_path->text());
	if (data4.isEmpty())
	{
		ui.progressBar->setVisible(false);
		return;
	}*/
	QStringList data5 = readTextFile(ui.lineEdit_CLE_save_path->text());
	if (data5.isEmpty())
	{
		ui.progressBar->setVisible(false);
		return;
	}
	ui.progressBar->setValue(10);

	// 创建应用程序,先用WPS,没有就用word                                                                                 
	std::unique_ptr<QAxObject> workapp(new QAxObject());
	bool workflag = workapp->setControl("KWPS.Application");
	if (!workflag)
	{
		workflag = workapp->setControl("Word.Application");
		if (!workflag)
		{
			QMessageBox::critical(nullptr, "错误", "无法创建 Word 或 WPS 应用程序对象！");
			ui.progressBar->setVisible(false);
			return;
		}
	}
	ui.progressBar->setValue(20);
	workapp->setProperty("Visible", false); // 后台运行
	ui.progressBar->setValue(35);


	// 打开模板文件
	std::unique_ptr<QAxObject> documents(workapp->querySubObject("Documents"));
	if (!documents)
	{
		QMessageBox::critical(this, "错误", "无法获取文档集合对象！");
		ui.progressBar->setVisible(false);
		workapp->dynamicCall("Quit()");
		return;
	}
	std::unique_ptr<QAxObject>  document(documents->querySubObject("Open(const QString&)", templatePath));
	if (!document)
	{
		QMessageBox::critical(this, "错误", " 无法打开模板文件，请检查文件是否存在或被占用！");
		ui.progressBar->setVisible(false);
		workapp->dynamicCall("Quit()");
		return;
	}
	//将.dotx文件保存为.docx文件
	QVariant result1 = document->dynamicCall("SaveAs2(const QString&,int)", savePath, 16);
	// 关闭.dotx文档
	document->dynamicCall("Close()");

	//打开.docx文档
	std::unique_ptr<QAxObject>  document2(documents->querySubObject("Open(const QString&)", savePath));
	if (!document2)
	{
		QMessageBox::critical(this, "错误", " 无法打开模板保存的.docx文件");
		ui.progressBar->setVisible(false);
		workapp->dynamicCall("Quit()");
		return;
	}

	insertTextByBookmark(document2.get(), "bookmark_number", b_number_8);
	insertTextByBookmark(document2.get(), "bookmark_type", type_last);
	insertTextByBookmark(document2.get(), "bookmark_model", name_last);
	insertTextByBookmark(document2.get(), "bookmark_device", instru_last);
	insertTextByBookmark(document2.get(), "bookmark_time", date_last);
	insertTextByBookmark(document2.get(), "bookmark_people", person_last);
	ui.progressBar->setValue(65);

	// 获取表格对象
	std::unique_ptr<QAxObject> tables(document2->querySubObject("Tables"));
	if (!tables)
	{
		QMessageBox::critical(this, "错误", "无法获取 Word 中表格！");
		document2->dynamicCall("Close()");
		workapp->dynamicCall("Quit()");
		ui.progressBar->setVisible(false);
		return;
	}
	std::unique_ptr<QAxObject> table1(tables->querySubObject("Item(int)", 1));
	std::unique_ptr<QAxObject> table2(tables->querySubObject("Item(int)", 2));
	std::unique_ptr<QAxObject> table3(tables->querySubObject("Item(int)", 3));
	std::unique_ptr<QAxObject> table4(tables->querySubObject("Item(int)", 4));
	std::unique_ptr<QAxObject> table5(tables->querySubObject("Item(int)", 5));
	ui.progressBar->setValue(75);

	// 向表格填充数据
	fillTableWithData(table1.get(), data1, 2, 2);
	fillTableWithData(table2.get(), data2, 2, 2);
	ui.progressBar->setValue(85);
	fillTableWithData(table3.get(), data3, 2, 2);
	//fillTableWithData(table4.get(), data4, 2, 2);
	fillTableWithData(table5.get(), data5, 2, 2);


	// 保存并退出
	document2->dynamicCall("Close(bool)", "true");
	//  document_wps2->dynamicCall("Save()");
	// /* QString savePath1= basePath + b_number_8 +"最终版" + ".docx";
	//  QVariant result2= document_wps2->dynamicCall("SaveAs2(const QString&,int)", savePath1  , 16);
	//*/
	//  
	//  document_wps2->dynamicCall("Close()");
	ui.progressBar->setValue(100);
	ui.progressBar->setVisible(false);
	if (QFile::exists(savePath) && saveornot)
	{
		QMessageBox::information(this, "成功", "文件保存成功！");
	}
	else
	{
		QMessageBox::critical(this, "错误", "文件保存失败！");
	}
	workapp->dynamicCall("Quit()");	
}

// 读取文本文件并处理数据
QStringList  WBalance::readTextFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, "错误", "无法打开文件：" + filePath);
		return {};
	}

	QTextStream in(&file);
	QStringList lines;
	while (!in.atEnd()) {
		lines.append(in.readLine());
	}
	file.close();

	lines.removeFirst();


   /*
   QStringList data;
   for (int i = 0; i < lines.size(); ++i)
	 {
		 QString line = lines[i];
		 QString newLine = line.left(line.length() - 1);
		 QStringList parts = newLine.split(' ', Qt::SkipEmptyParts);
		 for (int j = 0; j < parts.size(); ++j)
		 {
			 QString part = parts[j];
			 if (isNumeric(part))
			 {
				 data.append(part);
			 }
		 }
	 }*/

	QStringList data;
	for (int i = 0; i < lines.size(); ++i)
	{
		QString line = lines[i];
		//qDebug() << line;
		QString newLine = line.left(line.length() - 1);
		QTextStream stream(&newLine);
		QString part;
		while (!stream.atEnd())
		{
			stream >> part;
			if (isNumeric(part))
			{
			//	qDebug() << part;
				data.append(part);
			}
		}

	}
	//qDebug() << "结束                  1111111111111111111111111111111111111111111111111111111111111      ";
	return data;
}

// 填充表格数据
void WBalance::fillTableWithData(QAxObject* table, const QStringList& data, int startRow, int startCol)
{
	if (!table)
	{
		QMessageBox::warning(this, "警告", "表格不存在!");
		saveornot = false;
		return;
	};
	int index = 0;

	QAxObject* tableRows = table->querySubObject("Rows");
	QAxObject* tableCols = table->querySubObject("Columns");
	if (tableRows->isNull() || tableCols->isNull())
	{
		saveornot = false;
		return;
	}
	int rows = tableRows->dynamicCall("Count").toInt();
	int cols = tableCols->dynamicCall("Count").toInt();


	if ((rows - 1) * (cols - 1) != data.size())
	{
		QMessageBox::critical(this, "错误", "表格可容纳的数据量与实际数据量不匹配！");
		saveornot = false;
		return;
	}

	for (int row = startRow; row <= startRow + (data.size() / 6) - 1; ++row)
	{
		for (int col = startCol; col <= 7; ++col)
		{
			if (index >= data.size()) break;
			/* std::unique_ptr<QAxObject> cell(table->querySubObject("Cell(int, int)", row, col));
			 if (cell)
			 {
				 std::unique_ptr<QAxObject> range(cell->querySubObject("Range"));
				 if (range) {
					 range->setProperty("Text", data[index++]);
				 }
			 }*/
			QAxObject* cell = table->querySubObject("Cell(int, int)", row, col);
			if (!cell)
			{
				QMessageBox::warning(this, "警告", "无法获取表格单元格！行：" + QString::number(row) + "，列：" + QString::number(col));
				continue;
			}

			QAxObject* range = cell->querySubObject("Range");
			if (!range)
			{
				QMessageBox::warning(this, "警告", "无法获取单元格范围！行：" + QString::number(row) + "，列：" + QString::number(col));
				continue;
			}
			range->setProperty("Text", data[index++]);

		}
	}
}

// 通过书签插入文本
void WBalance::insertTextByBookmark(QAxObject* document, const QString& bookmarkName, const QString& text)
{
	if (!document) return;
	std::unique_ptr<QAxObject> bookmarks(document->querySubObject("Bookmarks"));
	std::unique_ptr<QAxObject> bookmark(bookmarks->querySubObject("Item(const QString&)", bookmarkName));
	if (bookmark)
	{
		std::unique_ptr<QAxObject> range(bookmark->querySubObject("Range"));
		if (range)
		{
			range->setProperty("Text", text);
			int start = range->property("Start").toInt();
			int End = start + text.length();
			std::unique_ptr<QAxObject> newrange(document->querySubObject("Range(int, int)", start, End));
			if (newrange)
			{
				// 重新添加同名书签到新范围，便于后续多次保存
				QVariant rangeVariant = QVariant::fromValue(newrange.get());
				bookmarks->dynamicCall("Add(const QString&, QVariant)", bookmarkName, rangeVariant);
			}
		}
	}
}


// 判断字符串是否为数字
bool WBalance::isNumeric(const QString& str)
{
	bool ok;
	str.toDouble(&ok);
	return ok;
}

//判断保持的文件是否被占用
bool WBalance::isFileLocked(const QString& filePath)
{
	QFile file(filePath);
	return !file.open(QIODevice::WriteOnly | QIODevice::Append);
}

void WBalance::quit()
{
    QApplication::quit();
}

void WBalance::closeEvent(QCloseEvent* event)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("确认退出");
	msgBox.setText("是否退出程序？");

	msgBox.setStyleSheet("QLabel {" " text-align: center;"  " qproperty-alignment: 'AlignHCenter';""}");

	//添加自定义按钮（中文）
	QPushButton* btnYes = msgBox.addButton("是", QMessageBox::YesRole);
	QPushButton* btnNo = msgBox.addButton("否", QMessageBox::NoRole);
	msgBox.setDefaultButton(btnNo); //设置默认选中“否”

	//显示对话框并等待用户操作
	msgBox.exec();

	//根据用户点击的按钮决定是否退出
	if (msgBox.clickedButton() == btnYes) 
	{
		event->accept(); //允许关闭窗口
	}
	else 
	{
		event->ignore(); //阻止关闭窗口
	}
}
