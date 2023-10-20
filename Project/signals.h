#ifndef SIGNALS_H
#define SIGNALS_H

#include "mainwindow.h"

void MainWindow::click_on_canvas( const QPoint &pos )
{
    double x = ui->plot->invTransform(QwtPlot::xBottom, pos.x());
    double y = ui->plot->invTransform(QwtPlot::yLeft, pos.y());

    statusBar()->showMessage("x= " + QString::number(x) +
                             "; y = " + QString::number(y));
}
bool MainWindow::on_mImport_triggered()
{
    fileNamefluids = QFileDialog::getOpenFileName(0, "Выбрать файл с исходными данными","*.xlsx", 0);
    QXlsx::Document xlsx(fileNamefluids);
    QStringList sheetNames = xlsx.sheetNames();
    if ( sheetNames.isEmpty() )
    {
        QMessageBox::critical( this, QAbstractTableModel::tr("Ошибка"), QAbstractTableModel::tr("Файл пустой. Данные для загрузки отсутствуют.") );
        qInfo().noquote() << QString(QAbstractTableModel::tr("Данные для загрузки в файле отсутствуют. Файл: %1")).arg(fileNamefluids);
        return false;
    }
    data.clear();
    QXlsx::CellRange range = xlsx.dimension();
    if ( range.columnCount() * range.rowCount() > 1)
    {
        for ( int row = 2, rows = range.rowCount(); row <= rows; row++ )
        {
            for ( int col = 1, cols = range.columnCount(); col < cols; col++ )
            {
                QXlsx::Cell *cell = xlsx.cellAt(row, col);
                double value = 0.0;
                switch (col)
                {
                    case Data_Sn:
                        if (getValueFromCell(cell, value))
                            data_point.Sn = value;
                        break;
                    case Data_kND:
                        if (getValueFromCell(cell, value))
                            data_point.kND = value;
                        break;
                    case Data_kNI:
                        if (getValueFromCell(cell, value))
                            data_point.kNI = value;
                        break;
                    case Data_kWD:
                        if (getValueFromCell(cell, value))
                        {
                            data_point.kWD = value;
                            count_I ++;
                        }
                        break;
                    case Data_kWI:
                        if (getValueFromCell(cell, value))
                            data_point.kWI = value;
                        break;
                }
            }
            data.push_back(data_point);
        }
    }
    addDataCurve();
    return true;
}
void MainWindow::on_revers_Button_clicked()
{
    QString spinBoxText = ui->revers_SpinBox->text().replace(
                QLocale().decimalPoint(), QLatin1Char('.'));
    double x = spinBoxText.toDouble();
    if(switchCalcType == 1)
    {
        calc_k(x);
        prepare_data();
    }
    else
    {
        if((x > numBetmin) && (x < numBetmax))
        {
            double x1, x2;
            numBet.push_back(x);
            // calc first line
            if((numBetCur % 2) == 0)
            {
                for(int i = 0; i < numBetCur; i++)
                {
                    x1 = numBet.at(i);
                    x2 = numBet.at(i + 1);
                    if((i % 2) == 0)
                    {
                        calc1_I(x1, x2, i);
                        calc1_D(x1, x2, i);
                    }
                    else
                    {
                        calc2_I(x2, x1);
                        calc2_D(x2, x1, i);
                    }
                }
                x1 = x;
                x2 = 0.0;
                calc1_I(x1, x2, numBetCur);
                calc1_D(x1, x2, numBetCur);
            }
            // calc second line
            if((numBetCur % 2) == 1)
            {
                for(int i = 0; i < numBetCur; i++)
                {
                    x1 = numBet.at(i);
                    x2 = numBet.at(i + 1);
                    if((i % 2) == 0)
                    {
                        calc1_I(x1, x2, i);
                        calc1_D(x1, x2, i);
                    }
                    else
                    {
                        calc2_I(x2, x1);
                        calc2_D(x2, x1, i);
                    }
                }
                x1 = 1.0;
                x2 = x;
                calc2_I(x1, x2);
                calc2_D(x1, x2, numBetCur);
            }
            numBetCur ++;
            prepare_data();
        }
    }
}
void MainWindow::on_mExport_triggered()
{
    QString fileName = "result.txt";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        int count = point_export_x1.count();
        for (int i = 0; i < count; ++i)
        {
            QString x1_s = QString::number(point_export_x1.at(i));
            QString y1_s = QString::number(point_export_y1.at(i));
            QString x2_s = QString::number(point_export_x2.at(i));
            QString y2_s = QString::number(point_export_y2.at(i));
            QString writeData = x1_s + " " + y1_s + " " + x2_s + " " + y2_s;
            stream << writeData << "\n";
        }
    }
    file.close();
}
void MainWindow::on_reset_data_Button_clicked()
{
    on_reset_result_Button_clicked();

    points1.clear();
    points2.clear();
    points3.clear();
    points4.clear();

    data.clear();

    d_point.clear();
    i_point.clear();
    d_point1.clear();
    i_point1.clear();

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
void MainWindow::on_reset_result_Button_clicked()
{
    calc_point_x.clear();
    calc_point_y.clear();
    write_revers_line1(calc_point_x, calc_point_y);

    calc_point1_x.clear();
    calc_point1_y.clear();
    write_revers_line2(calc_point1_x, calc_point1_y);
}
void MainWindow::prepare_data()
{
    write_data_for_export(calc_point_x, calc_point_y, calc_point1_x, calc_point1_y);

    write_revers_line1(calc_point_x, calc_point_y);
    calc_point_x.clear();
    calc_point_y.clear();

    write_revers_line2(calc_point1_x, calc_point1_y);
    calc_point1_x.clear();
    calc_point1_y.clear();
}
#endif // SIGNALS_H
