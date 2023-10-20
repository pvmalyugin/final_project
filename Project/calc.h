#ifndef CALC_H
#define CALC_H

#include "mainwindow.h"

void MainWindow::calc1_I(double x1, double x2, int numCur)
{
    int factor_int, sizeD;
    double k2, x_1, x_2, y_1, y_2, AB, BB, k1, krwip, A1;

    factor_int = factor(1.0 - x1);
    k2 = function2(factor_int, (1.0 - x1));
    sizeD = i_point1.count() - 1;
    x_1 = i_point.at(0);
    x_2 = i_point.at(sizeD);
    y_1 = i_point1.at(0);
    y_2 = i_point1.at(sizeD);
    AB = ((1.0 - x1) - x_1) / (x_2 - x_1);
    BB = (y_2 - y_1);
    k1 = y_1 + AB * BB;
    if(numCur == 0) krwip = k2;
    else
    {
        int count_point = calc_point_y.count() - 1;
        krwip = calc_point_y.at(count_point);
    }
    A1 = (k1 - krwip) / (k1 - k2);
    for (int i = 0; i <= count_r; i++)
    {
        double step, newX, Sw, k3, k4, A, B, C, krw;

        step = (x1 - x2)/count_r;
        newX = x1 - step * i;
        Sw = 1.0 - newX;
        AB = (Sw - x_1) / (x_2 - x_1);
        BB = (y_2 - y_1);
        k3 = y_1 + AB * BB;
        factor_int = factor(Sw);
        k4 = function2(factor_int, Sw);
        C = k3 - k4;
        B = (1.0 - Sw) / (1.0 - (1.0 - x1));
        A = A1 * B * C;
        krw = k3 - A;
        if(krw > 1.0)
            break;
        calc_point_x.push_back(newX);
        calc_point_y.push_back(krw);
    }
}
void MainWindow::calc1_D(double x1, double x2, int numCur)
{
    int factor_int, factor_int1, factor_int2;
    double f1, f6, kroip, A2, step, newX, Sw, A, B, C, kro;
    factor_int = factor(x1);
    f1 = function1(factor_int, x1);
    factor_int1 = factor3(x1);
    factor_int2 = factor2(x1 + d_point.at(0));
    f6 = function6(factor_int1, factor_int2, x1);
    if(numCur == 0) kroip = f1;
    else
    {
        int count_point = calc_point1_y.count() - 1;
        kroip = calc_point1_y.at(count_point);
    }
    A2 = (kroip - f6) / (f1 - f6);
    for (int i = 0; i <= count_r; i++)
    {
        step = (x1 - x2)/count_r;
        newX = x1 - step * i;
        Sw = newX;
        factor_int = factor(Sw);
        f1 = function1(factor_int, Sw);
        factor_int1 = factor3(Sw);
        factor_int2 = factor2(Sw + d_point.at(0));
        f6 = function6(factor_int1, factor_int2, Sw);
        C = f1 - f6;
        B = (1.0 - Sw) / (1.0 - x1);
        A = A2 * B * C;
        kro = f6 + A;
        if(kro < 0.0)
            break;
        calc_point1_x.push_back(newX);
        calc_point1_y.push_back(kro);
    }
}
void MainWindow::calc2_I(double x1, double x2)
{
    int factor_int, sizeD;
    double k2, x_1, x_2, y_1, y_2, AB, BB, k1, krwip, A1;
    int count_point = calc_point_y.count() - 1;
    krwip = calc_point_y.at(count_point);
    factor_int = factor(1.0 - x2);
    k2 = function2(factor_int, (1.0 - x2));
    sizeD = i_point1.count() - 1;
    x_1 = i_point.at(0);
    x_2 = i_point.at(sizeD);
    y_1 = i_point1.at(0);
    y_2 = i_point1.at(sizeD);
    AB = ((1.0 - x2) - x_1) / (x_2 - x_1);
    BB = (y_2 - y_1);
    k1 = y_1 + AB * BB;
    A1 = (krwip - k2) / (k1 - k2);
    for (int i = 0; i <= count_r; i++)
    {
        double step, newX, Sw, A, B, C, krw, k3, k4;
        step = (x1 - x2)/count_r;
        newX = (1.0 - x2) - step * i;
        Sw = newX;
        AB = (Sw - x_1) / (x_2 - x_1);
        BB = (y_2 - y_1);
        k3 = y_1 + AB * BB;
        factor_int = factor(Sw);
        k4 = function2(factor_int, Sw);
        C = k3 - k4;
        B = Sw / (1.0 - x2);
        A = A1 * B * C;
        krw = k4 + A;
        calc_point_x.push_back(1.0 - newX);
        calc_point_y.push_back(krw);
    }
}
void MainWindow::calc2_D(double x1, double x2, int numCur)
{
    int count_point = calc_point1_y.count() - 1;
    double kroip = calc_point1_y.at(count_point);
    double krod_c = qPow(x2, (4.0/3.0)) * 3.393022;
    double kroi_c = qPow(x2, (4.0/3.0));

    int factor_int = factor(x2);
    double f1 = function1(factor_int, x2);
    int factor_int1 = factor3(x2);
    int factor_int2 = factor2(x2 + d_point.at(0));
    double f6 = function6(factor_int1, factor_int2, x2);

//qDebug() <<    x2  << " : " << f1 << " = " << kroi_c;
//qDebug() << (1-x2) << " : " << f6 << " = " << krod_c;

    double A2 = (f6 - kroip) / (f6 - f1);
    double CC;
    if(numCur == 1)
        CC = 0.06;
    else
        CC = 0.1;
    for (int i = 0; i <= count_r; i++)
    {
        double step = (x1 - x2)/count_r;
        double newX = x2 + step * i;
        double Sw = newX;
        factor_int = factor2(Sw);
        f1 = function1(factor_int, Sw);
        double krod = qPow(Sw, (4.0/3.0)) * 3.393022;
if(Sw == 0.3)
{
    qDebug() << "....";
}
        factor_int1 = factor3(Sw);
        factor_int2 = factor2(Sw + d_point.at(0));
        f6 = function6(factor_int1, factor_int2, Sw);
        double kroi = qPow(Sw, (4.0/3.0));
//qDebug() << Sw << " :" << f1 << " = " << kroi;
qDebug() << Sw << " :" << f6 << " = " << krod;
        double C = f1 - krod;
        double B = 1.0 - (i * CC * 1 / count_r);
        double A = A2 * B * C;
        double kro = krod + A;
        calc_point1_x.push_back(newX);
        calc_point1_y.push_back(kro);
    }
}
int MainWindow::factor(double x)
{
    int factor = 0;
    int count = data.count();
    for (int i = 0; i < (count - 1); i ++)
    {
        if(x >= data.at(i).Sn && x < data.at(i + 1).Sn)
        {
            factor = i;
            break;
        }
    }
    return factor;
}
int MainWindow::factor2(double x)
{
    int factor = 0;
    int count = data.count();
    if(x >= data.at(count -1).Sn)
    {
        factor = count - 2;
        return factor;
    }
    for (int i = 0; i < (count - 1); i ++)
    {
        if(x >= data.at(i).Sn && x < data.at(i + 1).Sn)
        {
            factor = i;
            break;
        }
    }
    return factor;
}
int MainWindow::factor3(double x)
{
    int factor = 0;
    int count = i_point.count() - 1;
    if(x > i_point.at(count))
    {
        factor = count - 1;
        return factor;
    }
    count = data.count();
    for (int i = 0; i < (count - 1); i ++)
    {
        if(x >= data.at(i).Sn && x < data.at(i + 1).Sn)
        {
            factor = i;
            break;
        }
    }
    return factor;
}
double MainWindow::function1(int i, double x)
{
    double A = (x - data.at(i).Sn) / (data.at(i + 1).Sn - data.at(i).Sn);
    double B = data.at(i + 1).kND - data.at(i).kND;
    double result = data.at(i).kND + A * B;
    return result;
}
double MainWindow::function2(int i, double x)
{
    double A = (x - data.at(i).Sn) / (data.at(i + 1).Sn - data.at(i).Sn);
    double B = data.at(i + 1).kWI - data.at(i).kWI;
    double result = data.at(i).kWI + A * B;
    return result;
}
double MainWindow::function3(int i, double x)
{
    double A = (x - i_point.at(0)) / ((1.0 - i_point.at(i)) - i_point.at(0));
    int k = data.count() - 1;
    double B = i_point1.at(i) - data.at(k).kWI;
    double result = data.at(k).kWI + A * B;
    return result;
}
double MainWindow::function4(int i, double x)
{
    double A = (x - data.at(i).Sn) / (data.at(i + 1).Sn - data.at(i).Sn);
    double B = data.at(i + 1).kWD - data.at(i).kWD;
    double result = data.at(i).kWD + A * B;
    return result;
}
double MainWindow::function5(int i, double x)
{
    double A = (x - data.at(i).Sn) / (data.at(i + 1).Sn - data.at(i).Sn);
    double B = data.at(i + 1).kNI - data.at(i).kNI;
    double result = data.at(i).kNI + A * B;
    return result;
}
double MainWindow::function6(int i1, int i2, double x)
{
    double A = (x - data.at(i1).Sn) / (data.at(i1 + 1).Sn - data.at(i1).Sn);
    double B = data.at(i2 + 1).kNI - data.at(i2).kNI;
    double result = data.at(i2).kNI + A * B;
    return result;
}
void MainWindow::calc_k(double x)
{
    int sizeD = d_point.size() - 1;
    int sizeI = i_point.size() - 1;
    double C = (1.0 / d_point.at(0) - 1.0 / d_point.at(sizeD));
    double Snr = x/(1.0 + C * x);
    int factor_int = factor(x);
    double f1 = function1(factor_int, x);
    factor_int = factor(1.0 - x);
    double f2 = function2(factor_int, (1.0 - x));
    double f3 = function3(sizeD, Snr);
    double lambda = 1.2;

    int count = 100;
    for (int i = 0; i <= count; i++)
    {
        // calc first line
        double Sn = Snr + (x - Snr) / count * i;
        double k_ImbrN = f1 * qPow((Sn - Snr) / (x - Snr), lambda);
        calc_point_x.push_back(Sn);
        calc_point_y.push_back(k_ImbrN);

        // calc second line
        double SnormN = i_point.at(0) + (Sn - Snr) * (i_point.at(sizeD) - i_point.at(0)) / (x - Snr);
        factor_int = factor(i_point.at(sizeI) - SnormN);
        double f4 = function4(factor_int, i_point.at(sizeD) - SnormN);
        double A = f3 - f2;
        double B = (f4 - d_point1.at(0)) / (d_point.at(sizeD) - i_point1.at(sizeD));
        double C = f2 + A * B;
        calc_point1_x.push_back(Sn);
        calc_point1_y.push_back(C);
    }
}
#endif // CALC_H
