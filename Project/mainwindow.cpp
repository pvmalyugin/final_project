#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calc.h"
#include "signals.h"

#include <QList>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initAction();

    addPlot();
    addPlotGrid();
    enableMagnifier();
    enableMovingOnPlot();
    enablePicker();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addPlot()
{
    ui->plot->setTitle( "Гистерезис ОФП" );
    ui->plot->setCanvasBackground( Qt::white );

    ui->plot->setAxisTitle(QwtPlot::yLeft, "Relative permeability");
    ui->plot->setAxisTitle(QwtPlot::xBottom, "Water saturation");
    ui->plot->insertLegend( new QwtLegend() );

}
void MainWindow::addPlotGrid()
{
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QPen( Qt::gray, 0.1 ));
    grid->attach( ui->plot );
}
void MainWindow::addDataCurve()
{
    curve1 = new QwtPlotCurve();
    curve2 = new QwtPlotCurve();
    curve3 = new QwtPlotCurve();
    curve4 = new QwtPlotCurve();
    curve1->setTitle( "kND" );
    curve2->setTitle( "kNI" );
    curve3->setTitle( "kWD" );
    curve4->setTitle( "kWI" );
    curve1->setPen( Qt::green, 1 );
    curve2->setPen( Qt::green, 1 );
    curve3->setPen( Qt::red, 1 );
    curve4->setPen( Qt::red, 1 );

    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::black ), QPen( Qt::blue, 0 ), QSize( 1, 1 ) );
    curve1->setSymbol( symbol );
    curve2->setSymbol( symbol );
    curve3->setSymbol( symbol );
    curve4->setSymbol( symbol );

    int count = data.count();

    for (int i = 0; i < count; i++) {
        points1 << QPointF( data.at(i).Sn, data.at(i).kND);
        if(i > count_I) {
            points2 << QPointF( data.at(i).Sn, data.at(i).kNI);
            d_point << double (data.at(i).Sn);
            d_point1 << double (data.at(i).kNI); }
        if(i < count_I) {
            points3 << QPointF( 1.0 - data.at(i).Sn, data.at(i).kWD);
            i_point << double (data.at(i).Sn);
            i_point1 << double (data.at(i).kWD); }
        points4 << QPointF( 1.0 - data.at(i).Sn, data.at(i).kWI);
    }

    curve1->setSamples( points1 );
    curve2->setSamples( points2 );
    curve3->setSamples( points3 );
    curve4->setSamples( points4 );

    curve1->attach( ui->plot );
    curve2->attach( ui->plot );
    curve3->attach( ui->plot );
    curve4->attach( ui->plot );

    ui->plot->setAxisAutoScale(QwtPlot::yLeft, true);
    ui->plot->setAxisAutoScale(QwtPlot::xBottom, true);
    ui->plot->replot();
}
void MainWindow::enableMagnifier()
{
    QwtPlotMagnifier *magnifier =
            new QwtPlotMagnifier(ui->plot->canvas());
    magnifier->setMouseButton(Qt::MidButton);
}
void MainWindow::enableMovingOnPlot()
{
    QwtPlotPanner *d_panner = new QwtPlotPanner( ui->plot->canvas() );
    d_panner->setMouseButton( Qt::RightButton );
}
void MainWindow::enablePicker()
{
    QwtPlotPicker *d_picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
    QwtPlotPicker::CrossRubberBand,
    QwtPicker::AlwaysOn,
    ui->plot->canvas() );

    d_picker->setRubberBandPen( QColor( Qt::red ) );

    d_picker->setTrackerPen( QColor( Qt::black ) );

    d_picker->setStateMachine( new QwtPickerDragPointMachine() );

    connect( d_picker, SIGNAL( appended( const QPoint & ) ),
            SLOT( click_on_canvas( const QPoint & ) ) );
}
void MainWindow::write_revers_line(double value)
{
    if(isWrite)
    {
        curveX->detach();
        pointsX.clear();
    }
    curveX = new QwtPlotCurve();
    curveX->setTitle( "rev" );
    curveX->setPen( Qt::black, 1 );
    int count = 11;
    for (int i = 0; i < count; i++) {
        pointsX << QPointF( value, 0.1 * i);
    }
    curveX->setSamples( pointsX );
    curveX->attach( ui->plot );
    ui->plot->replot();
    isWrite = true;
}
void MainWindow::write_revers_line1(QVector<double> x, QVector<double> y)
{
    if(isWriteD)
    {
        curveD->detach();
        pointsD.clear();
    }
    curveD = new QwtPlotCurve();
    curveD->setTitle( "cur1" );
    curveD->setPen( Qt::blue, 1 );
    int count = x.count();
    for (int i = 0; i < count; i++)
    {
        pointsD << QPointF ( x.at(i), y.at(i));
    }
    curveD->setSamples( pointsD );
    curveD->attach( ui->plot );
    ui->plot->replot();
    isWriteD = true;
}
void MainWindow::write_revers_line2(QVector<double> x, QVector<double> y)
{
    if(isWriteI)
    {
        curveI->detach();
        pointsI.clear();
    }
    curveI = new QwtPlotCurve();
    curveI->setTitle( "cur2" );
    curveI->setPen( Qt::blue, 1 );
    int count = x.count();
    for (int i = 0; i < count; i++)
    {
        pointsI << QPointF ( x.at(i), y.at(i));
    }
    curveI->setSamples( pointsI );
    curveI->attach( ui->plot );
    ui->plot->replot();
    isWriteI = true;
}
void MainWindow::initAction()
{
    QActionGroup* mCalcTypeSwitch = new QActionGroup(ui->menu);
    ui->action_Killough->setActionGroup(mCalcTypeSwitch);
    ui->action_Beattie->setActionGroup(mCalcTypeSwitch);

    ui->action_Killough->setData(1);
    ui->action_Beattie->setData(2);

    QObject::connect(mCalcTypeSwitch, &QActionGroup::triggered,[this](QAction *act)
    {
        int CalcType = act->data().toInt();
        switch (CalcType)
        {
        case 1:
            {
                ui->plot->setTitle( "Гистерезис ОФП (модель Killough)" );
                switchCalcType = 1;
            }
            break;
        case 2:
            {
                ui->plot->setTitle( "Гистерезис ОФП (модель Beattie)" );
                switchCalcType = 2;
            }
            break;
        default: ui->plot->setTitle( "Гистерезис ОФП" );
            break;
        }
    });
}
void MainWindow::write_data_for_export(QVector<double> x1, QVector<double> y1, QVector<double> x2, QVector<double> y2)
{
    point_export_x1.clear();
    point_export_y1.clear();
    point_export_x2.clear();
    point_export_y2.clear();
    int count = x1.count();
    for (int i = 0; i < count; ++i)
    {
        point_export_x1.push_back(x1.at(i));
        point_export_y1.push_back(y1.at(i));
    }
    count = x2.count();
    for (int i = 0; i < count; ++i)
    {
        point_export_x2.push_back(x2.at(i));
        point_export_y2.push_back(y2.at(i));
    }
}
