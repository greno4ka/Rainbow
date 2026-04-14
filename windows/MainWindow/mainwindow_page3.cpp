#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_spinBox_wave_page3_valueChanged(int value)
{
    scene3->setWavelength(value);
    ui->horizontalSlider_wave_page3->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page3_valueChanged(int value)
{
    scene3->setWavelength(value);
    ui->spinBox_wave_page3->setValue(value);
}

void MainWindow::on_spinBox_number_of_beams_page3_valueChanged(int value)
{
    scene3->setNumberOfBeams(ui->spinBox_number_of_beams_page3->value());
    ui->horizontalSlider_number_of_beams_page3->setValue(value);
}

void MainWindow::on_horizontalSlider_number_of_beams_page3_valueChanged(int value)
{
    scene3->setNumberOfBeams(ui->spinBox_number_of_beams_page3->value());
    ui->spinBox_number_of_beams_page3->setValue(value);
}

void MainWindow::on_radioButton1_page3_clicked()
{
    scene3->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page3_clicked()
{
    scene3->setDisplayMode(2);
}

void MainWindow::on_checkBox_show_angle_page3_stateChanged(int value)
{
    scene3->setShowAngle(value);
}

void MainWindow::on_checkBox_show_rainbow_beams_page3_stateChanged(int value)
{
    scene3->setShowRainbow(value);
}

void MainWindow::on_pushButton_reset_settings_page3_clicked()
{
    scene3->resetScene();
    ui->spinBox_wave_page3->setValue(600);
    ui->horizontalSlider_wave_page3->setValue(600);
    ui->spinBox_number_of_beams_page3->setValue(30);
    ui->horizontalSlider_number_of_beams_page3->setValue(30);
    ui->radioButton1_page3->setChecked(1);
    ui->checkBox_show_angle_page3->setChecked(0);
    ui->checkBox_show_rainbow_beams_page3->setChecked(0);
}