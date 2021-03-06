#include <iostream>
#include<string>
#include<vector>
#include<cmath>
#include<map>
#include<algorithm>
#include<random>
#include<ctime>
#include<Eigen/Dense>

using namespace Eigen;
using namespace std;

const int dim = 1;//维度
const int p_num = 10;//粒子数量
const int iters = 100;//迭代次数
const int inf = 999999;//极大值
const double pi = 3.1415;
//定义粒子的位置和速度范围
const double v_max = 4;
const double v_min = -2;
const double pos_max = 2;
const double pos_min = -1;
//定义位置向量和速度向量
vector<double> pos;
vector<double> spd;
//定义粒子的历史最优位置和全局最优位置
vector<double>p_best;
double g_best;
//使用eigen库定义函数数值矩阵和位置矩阵
Matrix<double, iters, p_num> f_test;
Matrix<double, iters, p_num> pos_mat;
//定义适应度函数
double fun_test(double x)
{
    double res = x * x + 1;
    return res;
}
//初始化粒子群的位置和速度
void init() 
{
    //矩阵中所有元素初始化为极大值
    f_test.fill(inf);
    pos_mat.fill(inf);
    //生成范围随机数
    static mt19937 rng;
    static uniform_real_distribution<double> distribution1(-1, 2);
    static uniform_real_distribution<double> distribution2(-2, 4);
    for (size_t i = 0; i < p_num; ++i)
    {
        pos.push_back(distribution1(rng));
        spd.push_back(distribution2(rng));
    }
    vector<double >vec;
    for (size_t i = 0; i < p_num; ++i)
    {
        auto temp = fun_test(pos[i]);//计算函数值
        //初始化函数值矩阵和位置矩阵
        f_test(0, i) = temp;
        pos_mat(0, i) = pos[i];
        p_best.push_back(pos[i]);//初始化粒子的历史最优位置
    }
    ptrdiff_t minRow, minCol;
    f_test.row(0).minCoeff(&minRow,&minCol);//返回函数值矩阵第一行中极小值对应的位置
    g_best = pos_mat(minRow,minCol);//初始化全局最优位置
}

void PSO() 
{
    static mt19937 rng;
    static uniform_real_distribution<double> distribution(0, 1);
    for (size_t step = 1; step < iters; ++step)
    {
        for (size_t i = 0; i < p_num; ++i)
        {
            //更新速度向量和位置向量
            spd[i] = 0.5 * spd[i] + 2 * distribution(rng) * (p_best[i] - pos[i]) + 2 * distribution(rng) * (g_best - pos[i]);
            pos[i] = pos[i] + spd[i];
            //如果越界则取边界值
            if (spd[i]<-2||spd[i]>4)
            {
                spd[i] = 4;
            }
            if (pos[i]<-1||pos[i]>2)
            {
                pos[i] = -1;
            }
            //更新位置矩阵
            pos_mat(step, i) = pos[i];
        }
        //更新函数值矩阵
        for (size_t i = 0; i < p_num; ++i)
        {
            auto temp = fun_test(pos[i]);
            f_test(step, i) = temp;
        }
        for (size_t i = 0; i < p_num; ++i)
        {
            MatrixXd temp_test;
            temp_test = f_test.col(i);//取函数值矩阵的每一列
            ptrdiff_t minRow, minCol;
            temp_test.minCoeff(&minRow,&minCol);//获取每一列的极小值对应的位置
            p_best[i] = pos_mat(minRow,i);//获取每一列的极小值，即每个粒子的历史最优位置
        }
        g_best = *min_element(p_best.begin(), p_best.end());//获取全局最优位置
    }
    cout << fun_test(g_best);
}
int main()
{
    init();
    PSO();
    cout << "\n**********************" << endl;
    system("pause");
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
