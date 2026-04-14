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

void MainWindow::on_radioButton_1rainbow_page2_clicked()
{
    scene2->setDisplayMode(1);
}

void MainWindow::on_radioButton_2rainbow_page2_clicked()
{
    scene2->setDisplayMode(2);
}

void MainWindow::on_pushButton_reset_settings_page2_clicked()
{
    double d=SCENE2_DISTANCE_086;
    int wl=SCENE2_WAVELENGTH_600;
    ui->doubleSpinBox_dist_page2->setValue(d);
    ui->horizontalSlider_dist_page2->setValue(d*100);
    ui->spinBox_wave_page2->setValue(wl);
    ui->horizontalSlider_wave_page2->setValue(wl);
    ui->radioButton_1rainbow_page2->setChecked(1);
    scene2->setDistance(d);
    scene2->setWavelength(wl);
    scene2->setDisplayMode(1);
}