#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton0_page5_clicked()
{
    scene5->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page5_clicked()
{
    scene5->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page5_clicked()
{
    scene5->setDisplayMode(2);
}

void MainWindow::on_spinBox_number_of_beams_page5_valueChanged(int value)
{
    scene5->setNumberOfBeams(value);
    ui->horizontalSlider_number_of_beams_page5->setValue(value);
}

void MainWindow::on_horizontalSlider_number_of_beams_page5_valueChanged(int value)
{
    scene5->setNumberOfBeams(value);
    ui->spinBox_number_of_beams_page5->setValue(value);
}

void MainWindow::on_spinBox_dispersion_quality_page5_valueChanged(int value)
{
    scene5->setBeamQuality(value);
    ui->horizontalSlider_dispersion_quality_page5->setValue(value);
}

void MainWindow::on_horizontalSlider_dispersion_quality_page5_valueChanged(int value)
{
    scene5->setBeamQuality(value);
    ui->spinBox_dispersion_quality_page5->setValue(value);
}

void MainWindow::on_pushButton_reset_settings_page5_clicked()
{
    scene5->setDisplayMode(0);
    scene5->setBeamQuality(10);
    scene5->setNumberOfBeams(30);
    ui->spinBox_dispersion_quality_page5->setValue(10);
    ui->horizontalSlider_dispersion_quality_page5->setValue(10);
    ui->spinBox_number_of_beams_page5->setValue(30);
    ui->horizontalSlider_number_of_beams_page5->setValue(30);
    ui->radioButton0_page5->setChecked(1);
}