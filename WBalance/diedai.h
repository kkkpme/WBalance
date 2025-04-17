//迭代过程
#include<cmath>
#include "../eigen-3.4.0/Eigen/Dense"
#include "../eigen-3.4.0/Eigen/Core"
#include<vector>
#include<map>
#include <string>
#include <regex>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <tuple>
#include <sstream>
#include <filesystem>
using namespace std;
using namespace Eigen;

vector<string> readFirstLine(const string&filePath)
{ 
    std::filesystem::path fsPath;
    try {
         // Use u8path to interpret the input string as UTF-8
        fsPath = std::filesystem::u8path(filePath);
    } catch (const std::exception& e) {
        cerr << "Error: Invalid file path format: " << filePath << " - " << e.what() << endl;
        return {};
    }
    ifstream inFile(fsPath);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open file " << fsPath << endl;
        return {};
    }

    string firstLine;
    if (getline(inFile, firstLine))
    {
        stringstream ss(firstLine);
        vector<string> headers;
        string header;
        while (ss >> header)
        {
            headers.push_back(header);
        }
        return headers;
    }
}

map<int, vector<double>> read_all_data(const string& dataFilePath)
{
    std::filesystem::path fsPath;
    try {
         // Use u8path to interpret the input string as UTF-8
        fsPath = std::filesystem::u8path(dataFilePath);
    } catch (const std::exception& e) {
        cerr << "Error: Invalid file path format: " << dataFilePath << " - " << e.what() << endl;
        return map<int, vector<double>>();
    }
    //string utf8Path = convertPathToUTF8(dataFilePath);


    ifstream inFile(fsPath);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open file " << fsPath << endl;
        return map<int, vector<double>>();
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line)) {
        if(line.find_first_not_of(" \t") == string::npos)
        {
            continue;
        }
        lines.push_back(line);
    }
    inFile.close();

    if (lines.empty()) {
        cerr << "Error: File is empty" << endl;
        return map<int, vector<double>>();
    }

    //如果第一行是标题行，可以删除（根据实际情况决定）
    lines.erase(lines.begin());

    map<int, vector<double>> data; 
    int dataIndex = 0;
    for(const auto& it:lines)
    {
        stringstream ss(it);
        int rowNum;
        ss >> rowNum; // 跳过行号

        vector<double> row;
        double value;
        while (ss >> value) 
        {
            row.push_back(value);
        }
        data[dataIndex] = row;                   // 偶数行
        dataIndex++;
    }
    return data;
}

void print_result(map<int, vector<double>> result)//打印结果，用来自己看的，这个函数可以删掉
{
    for(auto& it:result)
    {
        int key = it.first;
        const auto&row = it.second;
        cout << "  Key: " << key << ", Values: [";
        for(auto& value:row)
        {
            cout << setprecision(9)<<value << ", ";
        }
        cout << "]" << endl;
    }
}

pair<map<int, vector<double>>, map<int, vector<double>>> datalines(const string& dataFilePath) //读数据文件,返回两个map，第一个map存放非0载荷数据，第二个map存放0载荷数据
{
    std::filesystem::path fsPath;
    try {
         // Use u8path to interpret the input string as UTF-8
        fsPath = std::filesystem::u8path(dataFilePath);
    } catch (const std::exception& e) {
        cerr << "Error: Invalid file path format: " << dataFilePath << " - " << e.what() << endl;
        return make_pair(map<int, vector<double>>(), map<int, vector<double>>());
    }
    //string utf8Path = convertPathToUTF8(dataFilePath);


    ifstream inFile(fsPath);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open file " << fsPath << endl;
        return make_pair(map<int, vector<double>>(), map<int, vector<double>>());
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line)) {
        if(line.find_first_not_of(" \t") == string::npos)
        {
            continue;
        }
        lines.push_back(line);
    }
    inFile.close();

    if (lines.empty()) {
        cerr << "Error: File is empty" << endl;
        return make_pair(map<int, vector<double>>(), map<int, vector<double>>());
    }

    //如果第一行是标题行，可以删除（根据实际情况决定）
    lines.erase(lines.begin());

    map<int, vector<double>> data;         // 偶数行（非0载荷数据）
    map<int, vector<double>> zero_data; // 奇数行（0载荷数据）
    int dataIndex = 0;                     // data 的键
    int zero_dataIndex = 0;            // zero_fix_data 的键

    for (int i = 0; i < lines.size(); ++i) {
        stringstream ss(lines[i]);
        int rowNum;
        ss >> rowNum; // 跳过行号

        vector<double> row;
        double value;
        while (ss >> value) {
            row.push_back(value);
        }

        if (i % 2 == 0) {
            zero_data[zero_dataIndex] = row; // 奇数行（从0计数）
            zero_dataIndex++;
        } else {
            data[dataIndex] = row;                   // 偶数行
            dataIndex++;
        }
    }

    return make_pair(data, zero_data);
}

map<int ,vector<double>> coffdata(const string& coffFilePath)//读系数文件，并转换成6*27，方便后续使用
{
    std::filesystem::path fsPath;
    try {
         // Use u8path to interpret the input string as UTF-8
        fsPath = std::filesystem::u8path(coffFilePath);
    } catch (const std::exception& e) {
        cerr << "Error: Invalid file path format: " << coffFilePath << " - " << e.what() << endl;
            return map<int, vector<double>>();
    }
    ifstream inFile(fsPath);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open file " << fsPath << endl;
        return map<int, vector<double>>();
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line)) {
        if(line.find_first_not_of(" \t") == string::npos)
        {
            continue;
        }
        lines.push_back(line);
    }
    inFile.close();

    if (lines.empty()) {
        cerr << "Error: File is empty" << endl;
        return map<int, vector<double>>();
    }

    //如果第一行是标题行，可以删除（根据实际情况决定）
    lines.erase(lines.begin());

    map<int, vector<double>> data;
    int dataIndex = 0;

    for (int i = 0; i < lines.size(); ++i) {
        stringstream ss(lines[i]);
        vector<double> row;
        double value;
        while (ss >> value) {
            row.push_back(value);
        }
        data[dataIndex] = row;
        dataIndex++;
    }

    map<int, vector<double>> data_T;
    for(int j =0;j<6;j++)
    {
        vector<double> row;
        for(int i=0;i<data.size();i++)
        {
            row.push_back(data[i][j]);
        }
        data_T[j] = row;
    }

    return data_T;  
}

map<int, vector<double>> func_f (vector<double> Y)//计算干扰项的自变量，按顺序放
{
    map<int, vector<double>> m1;//一阶项
    for(int i=0;i<6;i++)
    {
        m1[i] = vector<double>{Y[0], Y[1], Y[2], Y[3], Y[4], Y[5]};
        m1[i].erase(m1[i].begin()+i);//剔除对角线上的自变量
    }

    vector<double> m2;//二阶项
    for(int i=0;i<6;i++)
    {
        m2.push_back(Y[i]*Y[i]);
    }
    for(int j=0;j<5;j++)
    {
        for(int k=j+1;k<6;k++)
        {
            m2.push_back(Y[j]*Y[k]);
        }
    }
    for(auto& it:m1)
    {
        it.second.insert(it.second.end(),m2.begin(),m2.end());
    }

    return m1;//返回自变量矩阵
}

map<int, vector<double>> diedai(map<int, vector<double>> coff,vector<double> U)
{
    /*
    coff：6*27个系数
    U：各载荷下产生的电压
    */
    //计算主项力Y_0
    vector <double> Y_0(6,0);
    for(int i=0;i<6;i++)
    {
        Y_0[i] = coff[i][i]*U[i];
    }
    // cout << "Y_0: " << Y_0[0] << " " << Y_0[1] << " " << Y_0[2] << " " << Y_0[3] << " " << Y_0[4] << " " << Y_0[5] << endl;
    // cout<<"==================================================================================="<<endl;
    map<int, vector<double>> tmp_coff = coff;
    int index = 0;//剔除对角线上的系数，只保留干扰项的系数
    for (auto& it:tmp_coff)
    {
        it.second.erase(it.second.begin()+index);
        index++;
    }
    // print_result(tmp_coff);//打印剔除对角线后的系数矩阵
    // cout<<"==================================================================================="<<endl;
    //迭代过程
    map<int, vector<double>> Y_i;//存放迭代结果

    vector <double> sum(6,0);//每个载荷的干扰项值
    map<int, vector<double>> x = func_f(Y_0);//利用Y_0计算出自变量矩阵
    // print_result(x);//打印自变量矩阵
    // cout<<"==================================================================================="<<endl;
    vector<double> tmp;
    for (int i=0;i<6;i++)
    {
        for (int j=0;j<tmp_coff[i].size();j++)
        {
            sum[i] += x[i][j] * tmp_coff[i][j] ;//将自变量与对应的系数相乘得到各载荷的干扰项值
        }
        tmp.push_back(Y_0[i] + sum[i]);
    }
    // cout<<"sum: "<<sum[0]<<" "<<sum[1]<<" "<<sum[2]<<" "<<sum[3]<<" "<<sum[4]<<" "<<sum[5]<<endl;
    // cout<<"tmp: "<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<" "<<tmp[4]<<" "<<tmp[5]<<endl;
    // cout<<"==================================================================================="<<endl;
    Y_i[0] = tmp;//第一次迭代结果

    for(int i=0;i<6;i++)//上面已经迭代一次了，这里再迭代6次
    {
        vector<double> sum1(6,0);
        map<int, vector<double>> x = func_f(Y_i[i]);//使用上一次的载荷计算下一次的自变量矩阵
        vector<double> tmp1;
        for (int j=0;j<6;j++)
        {
            for (int k=0;k<tmp_coff[j].size();k++)
            {
                sum1[j] += x[j][k] * tmp_coff[j][k] ;
            }
            tmp1.push_back(Y_0[j] + sum1[j]);
        }
        // cout<<"sum: "<<sum1[0]<<" "<<sum1[1]<<" "<<sum1[2]<<" "<<sum1[3]<<" "<<sum1[4]<<" "<<sum1[5]<<endl;
        // cout<<"tmp: "<<tmp1[0]<<" "<<tmp1[1]<<" "<<tmp1[2]<<" "<<tmp1[3]<<" "<<tmp1[4]<<" "<<tmp1[5]<<endl;
        // cout<<"==================================================================================="<<endl;
        Y_i[i+1] = tmp1;
    }

    return Y_i;//返回迭代结果
}



/*电压修正函数*/
map<int, vector<double>> fix_u(map<int, vector<double>> &data,map<int, vector<double>> &zero_load_data_u)//data是待修正电压
{
    vector<double> zero_U(6,0);//0载荷电压平均值，用于修正
    for(int i=0;i<6;i++)
    {
        double sum = 0;
        for(auto& it:zero_load_data_u)
        {
            sum += it.second[i];
        }
        zero_U[i] = sum/zero_load_data_u.size();
    }
    for(int j=0;j<6;j++)
    {
        for(auto&it :data)
        {
            it.second[j] -= zero_U[j];
        }
    }
    return data;
}

pair<map<int ,vector<double>>,vector<map<int ,vector<double>>>> _Result(string dataFilePath,string coffFilePath)//给出迭代结果
{
    vector<map<int ,vector<double>>> result_all;//存放迭代最终结果
    map<int, vector<double>> result_seven; 
    pair<map<int, vector<double>>,map<int, vector<double>>> data = datalines(dataFilePath);//读数据文件
    map<int ,vector<double>> data_xz = fix_u(data.first,data.second);//修正电压值
    map<int, vector<double>> coff = coffdata(coffFilePath);//6*27
    for (auto& it:data_xz)
    {
        vector<double> U = it.second;
        map<int, vector<double>> Y_i = diedai(coff,U);
        result_all.push_back(Y_i);
        result_seven[it.first] = Y_i[6];
    }
    return make_pair(result_seven, result_all);
}

struct Stats {
    double mean;     // 均值
    double stddev;   // 标准差
    Stats(double m = 0.0, double s = 0.0) : mean(m), stddev(s) {}
};

std::vector<Stats> calculateColumnStats(const std::map<int, std::vector<double>>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Input data is empty.");
    }

    // 找到所有行的最大列数
    size_t maxCols = 0;
    for (const auto& pair : data) {
        maxCols = std::max(maxCols, pair.second.size());
    }
    if (maxCols == 0) {
        throw std::invalid_argument("No valid columns found.");
    }

    // 初始化结果向量
    std::vector<Stats> results(maxCols);

    // 计算每一列的均值
    std::vector<double> sums(maxCols, 0.0);
    std::vector<int> counts(maxCols, 0); // 记录每列的有效元素个数

    for (const auto& pair : data) {
        const std::vector<double>& row = pair.second;
        for (size_t col = 0; col < row.size(); ++col) {
            sums[col] += row[col];
            counts[col]++;
        }
    }

    for (size_t col = 0; col < maxCols; ++col) {
        if (counts[col] == 0) {
            throw std::runtime_error("Column " + std::to_string(col) + " has no data.");
        }
        results[col].mean = sums[col] / counts[col];
    }

    // 计算每一列的标准差
    std::vector<double> sumSquaredDiff(maxCols, 0.0);
    for (const auto& pair : data) {
        const std::vector<double>& row = pair.second;
        for (size_t col = 0; col < row.size(); ++col) {
            double diff = row[col] - results[col].mean;
            sumSquaredDiff[col] += diff * diff;
        }
    }

    for (size_t col = 0; col < maxCols; ++col) {
        if (counts[col] < 2) {
            results[col].stddev = 0.0; // 只有一个元素，标准差定义为 0
        } else {
            double variance = sumSquaredDiff[col] / (counts[col] - 1); // 样本标准差
            results[col].stddev = std::sqrt(variance);
        }
    }

    return results;
}

vector<Stats> cfx(string dataFilePath,string coffFilePath)//data00
{
    pair<map<int ,vector<double>>,vector<map<int ,vector<double>>>> diedai_result= _Result(dataFilePath,coffFilePath);
    map<int ,vector<double>> result_seven= diedai_result.first;//7次迭代结果的第7次结果
    vector<Stats> cfx_r = calculateColumnStats(result_seven);//计算6分量的均值和标准差
    return cfx_r;
}

MatrixXd map_to_mat(map<int ,vector<double>> &data)//将map<int ,vector<double>>数据转化为MatrixXd数据
{
    int rowCount = data.size();
    int colCount = data.begin()->second.size();
    MatrixXd result(rowCount, colCount);
    int index = 0;
    for (const auto& pair : data) 
    {
        for (int j = 0; j < colCount; j++) 
        {
            result(index, j) = pair.second[j];
        }
        index++;
    }
    return result;
}

MatrixXd compute_66_matrix(string &dataFilePath,string &loadFilePath,string &coffFilePath)//计算66矩阵
{
    pair<map<int ,vector<double>>,vector<map<int ,vector<double>>>> diedai_result= _Result(dataFilePath,coffFilePath);
    map<int ,vector<double>> result_seven= diedai_result.first;//7次迭代结果的第7次结果
    MatrixXd result_seven_m = map_to_mat(result_seven);//迭代结果矩阵，也是自变量矩阵
    
    pair<map<int, vector<double>>,map<int, vector<double>>> data = datalines(loadFilePath);
    map<int, vector<double>> data00_f = data.first;//非0载荷数据
    MatrixXd data00_f_m = map_to_mat(data00_f);//加载矩阵
    MatrixXd delta_f = result_seven_m - data00_f_m;//ΔF作为因变量矩阵
    
    MatrixXd c = MatrixXd::Zero(delta_f.cols(),delta_f.cols());//存放系数矩阵
    for(int i =0;i<delta_f.cols();i++)
    {
        c.col(i) = (result_seven_m.transpose()*result_seven_m).inverse()*result_seven_m.transpose()*delta_f.col(i);
    }
    return c;
}
// 计算每一列的均值和标准差（vector 为行）


void printResult(const std::pair<std::map<int, std::vector<double>>, std::vector<std::map<int, std::vector<double>>>>& result) {
    // 打印第一个元素：map<int, vector<double>>
    std::cout << "First element (map<int, vector<double>>):\n";
    for (const auto& elem : result.first) {
        std::cout << "Key: " << elem.first << ", Values: ";
        for (double val : elem.second) {
            std::cout <<fixed<<setprecision(4)<< val << " ";
        }
        std::cout << "\n";
    }

    // // 打印第二个元素：vector<map<int, vector<double>>>
    // std::cout << "Second element (vector<map<int, vector<double>>>):\n";
    // for (size_t i = 0; i < result.second.size(); ++i) {
    //     std::cout << "Map " << i + 1 << ":\n";
    //     const auto& map = result.second[i];
    //     for (const auto& elem : map) {
    //         std::cout << "  Key: " << elem.first << ", Values: ";
    //         for (double val : elem.second) {
    //             std::cout <<fixed << setprecision(4)<< val << " ";
    //         }
    //         std::cout << "\n";
    //     }
    // }
}

vector<double> A2244(string &dataFilePath,string &loadFilePath,string &coffFilePath)
{
    pair<map<int ,vector<double>>,vector<map<int ,vector<double>>>> diedai_result= _Result(dataFilePath,coffFilePath);
    map<int ,vector<double>> result_seven= diedai_result.first;//7次迭代结果的第7次结果
    MatrixXd result_seven_m = map_to_mat(result_seven);//迭代结果矩阵，也是自变量矩阵
    
    pair<map<int, vector<double>>,map<int, vector<double>>> data = datalines(loadFilePath);
    map<int, vector<double>> data00_f = data.first;//非0载荷数据
    MatrixXd data00_f_m = map_to_mat(data00_f);//加载矩阵
    MatrixXd delta_f = result_seven_m - data00_f_m;

    vector<double> result;
    for(int col = 0 ;col<delta_f.cols();++col)
    {
        double sum = 0;
        for(int row = 0;row<delta_f.rows();++row)
        {
            sum += delta_f(row,col)*delta_f(row,col);
        }
        result.push_back(sqrt(sum/delta_f.rows()));
    }
    return result;
}


MatrixXd relative_error(MatrixXd &a,MatrixXd &b)
{
    MatrixXd resuls = MatrixXd::Zero(a.rows(),a.cols());
    for(int row = 0;row<a.rows();++row)
    {
        for(int col = 0;col<a.cols();++col)
        {
            resuls(row,col) = (a(row,col)-b(row,col))/b(row,col)*100;
        }
    }
    return resuls;
}


//int main()
//{
//    string dataFilePath = "F:/toHFUT/third/data02.dat";
//   // auto data = read_all_data(dataFilePath);
//   // print_result(data);
//    //string coffFilePath = "F:/toHFUT_V2/chose_method/delete_zero_unequal.txt";
//    string coffFilePath = "F:/toHFUT/third/coff.dat";
//    string loadFilePath = "F:/toHFUT/third/load02.dat";
//    // auto d1 = datalines(dataFilePath);
//    // auto d2 = coffdata(coffFilePath);
//    // auto d3 = datalines(loadFilePath);
//
//    auto m = compute_66_matrix(dataFilePath,loadFilePath,coffFilePath);
//    cout<<m<<endl;
//
//
//
//    // MatrixXd mat_result_seven = map_to_mat(result.first);
//    // pair<map<int, vector<double>>, map<int, vector<double>>> data = datalines(dataFilePath);
//    // MatrixXd mat_data_f = map_to_mat(data.first);
//    // cout<<mat_data_f<<endl;
//    // cout<<relative_error(mat_result_seven,mat_data_f)<<endl;
//
//
//    // pair<map<int, vector<double>>,map<int, vector<double>>> data = datalines(dataFilePath);//读数据文件
//    // map<int ,vector<double>> data_xz = fix_u(data.first,data.second);//修正电压值
//    // map<int, vector<double>> coff = coffdata(coffFilePath);//6*27
//    // vector<double> U = data_xz[0];
//    // map<int, vector<double>> Y_i = diedai(coff,U);
//
//    // map<int, vector<double>> r = func_f(data.first[0]);
//
//    // MatrixXd mat_r = map_to_mat(r);
//    // cout<<mat_r.row(0).transpose()<<endl;
//
//    return 0;
//}