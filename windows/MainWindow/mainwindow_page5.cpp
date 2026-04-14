#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton_both_rainbows_page5_clicked()
{
    scene5->setDisplayMode(0);
}

void MainWindow::on_radioButton_1rainbow_page5_clicked()
{
    scene5->setDisplayMode(1);
}

void MainWindow::on_radioButton_2rainbow_page5_clicked()
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
    ui->spinBox_number_of_beams_page5->setValue(SCENE5_NUMBER_OF_BEAMS);
    ui->horizontalSlider_number_of_beams_page5->setValue(SCENE5_NUMBER_OF_BEAMS);
    ui->spinBox_dispersion_quality_page5->setValue(SCENE5_BEAM_QUALITY_10);
    ui->horizontalSlider_dispersion_quality_page5->setValue(SCENE5_BEAM_QUALITY_10);
    scene5->setNumberOfBeams(SCENE5_NUMBER_OF_BEAMS);
    scene5->setBeamQuality(SCENE5_BEAM_QUALITY_10);
    scene5->setDisplayMode(0);
    ui->radioButton_both_rainbows_page5->setChecked(1);
}