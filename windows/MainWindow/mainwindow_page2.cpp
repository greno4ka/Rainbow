#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_doubleSpinBox_dist_page2_valueChanged(double value)
{
    scene2->setDistance(value);
    ui->horizontalSlider_dist_page2->setValue(value*100);
}

void MainWindow::on_horizontalSlider_dist_page2_valueChanged(int value)
{
    scene2->setDistance((double)value/100);
    ui->doubleSpinBox_dist_page2->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page2_valueChanged(int value)
{
    scene2->setWavelength(value);
    ui->horizontalSlider_wave_page2->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page2_valueChanged(int value)
{
    scene2->setWavelength(value);
    ui->spinBox_wave_page2->setValue(value);
}

void MainWindow::on_radioButton1_page2_clicked()
{
    scene2->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page2_clicked()
{
    scene2->setDisplayMode(2);
}

void MainWindow::on_pushButton_clearScr_page2_clicked()
{
    double d=0.86; // good inception with parallel beams
    int wl=600;
    ui->doubleSpinBox_dist_page2->setValue(d);
    ui->horizontalSlider_dist_page2->setValue(d*100);
    ui->spinBox_wave_page2->setValue(wl);
    ui->horizontalSlider_wave_page2->setValue(wl);
    ui->radioButton1_page2->setChecked(1);
    scene2->setDisplayMode(1);
}