#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMath>
#include <QDebug>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <QToolButton>
#include <QHBoxLayout>
#include <qwt_counter.h>

#include <QFileDialog>
#include "QMessageBox"

#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"

template<typename T>
bool getValueFromVariant(const QVariant &value, T &result)
{
    if (value.isNull()) return false;

    int typeId = qMetaTypeId< T >();
    if (typeId == QMetaType::UnknownType)
        return false;

    if ( !value.canConvert(typeId) )
        return false;

    result = value.value<typename std::decay<T>::type>();
    return true;
}
template<typename T>
bool getValueFromCell(QXlsx::Cell *cell, T &result)
{
    if (!cell) return false;
    return getValueFromVariant(cell->value(),result);
}

struct start_data
{
    double Sn; // насыщенность
    double kND; // офп несмачиваемой фазы дренаж
    double kNI; // офп несмачиваемой фазы пропитка
    double kWD; // офп смачиваемой фазы дренаж
    double kWI; // офп смачиваемой фазы пропитка
};

enum Column
{
    Data_Sn = 1,
    Data_kND,
    Data_kNI,
    Data_kWD,
    Data_kWI
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void click_on_canvas( const QPoint &pos );

    bool on_mImport_triggered();
    void on_revers_Button_clicked();
    void on_mExport_triggered();
    void on_reset_result_Button_clicked();
    void on_reset_data_Button_clicked();

private:

    Ui::MainWindow *ui;

    void initAction();
    void addPlot();
    void addPlotGrid();
    void enableMagnifier();
    void enableMovingOnPlot();
    void enablePicker();

    QString fileNamefluids;

    int count_I = 0;         // количество точек на дренажной кривой
    int count_r = 100;       // количество расчетных точек

    start_data data_point;
    QVector<start_data> data;
    void addDataCurve();
    QwtPlotCurve *curve1;
    QwtPlotCurve *curve2;
    QwtPlotCurve *curve3;
    QwtPlotCurve *curve4;
    QPolygonF points1;
    QPolygonF points2;
    QPolygonF points3;
    QPolygonF points4;
    QVector<double> d_point;
    QVector<double> i_point;
    QVector<double> d_point1;
    QVector<double> i_point1;

    void calc_k(double x);
    void calc1_I(double x1, double x2, int numCur);
    void calc2_I(double x1, double x2);

    void calc1_D(double x1, double x2, int numCur);
    void calc2_D(double x1, double x2, int numCur);

    void prepare_data();

    int factor(double x);
    int factor2(double x);
    int factor3(double x);
    double function1(int i, double x);
    double function2(int i, double x);
    double function3(int i, double x);
    double function4(int i, double x);
    double function5(int i, double x);
    double function6(int i1, int i2, double x);

    void write_revers_line(double value);
    QPolygonF pointsX;
    QwtPlotCurve *curveX;
    bool isWrite = false;

    QVector<double> calc_point_x;
    QVector<double> calc_point_y;
    void write_revers_line1(QVector<double> x, QVector<double> y);
    QPolygonF pointsD;
    QwtPlotCurve *curveD;
    bool isWriteD = false;

    QVector<double> calc_point1_x;
    QVector<double> calc_point1_y;
    void write_revers_line2(QVector<double> x, QVector<double> y);
    QPolygonF pointsI;
    QwtPlotCurve *curveI;
    bool isWriteI = false;

    int switchCalcType = 1;
    int numBetCur = 0;
    double numBetmin = 0.0;     // диапазон изменения насыщенности для
    double numBetmax = 1.0;     // следующей точки при модели Beattie
    QVector<double> numBet;     // вектор из точек для метода Beattie

    QVector<double> point_export_x1;
    QVector<double> point_export_y1;
    QVector<double> point_export_x2;
    QVector<double> point_export_y2;
    void write_data_for_export(QVector<double> x1, QVector<double> y1, QVector<double> x2, QVector<double> y2);
};

#endif // MAINWINDOW_H
