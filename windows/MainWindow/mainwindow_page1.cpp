#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_doubleSpinBox_dist_page1_valueChanged(double value)
{
    ui->horizontalSlider_dist_page1->setValue(value*100);
}

void MainWindow::on_horizontalSlider_dist_page1_valueChanged(int value)
{
    ui->doubleSpinBox_dist_page1->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page1_valueChanged(int value)
{
    ui->horizontalSlider_wave_page1->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page1_valueChanged(int value)
{
    ui->spinBox_wave_page1->setValue(value);
}

void MainWindow::on_radioButton_all_beams_page1_clicked()
{
    scene1->setDisplayMode(0);
}

void MainWindow::on_radioButton_1rainbow_page1_clicked()
{
    scene1->setDisplayMode(1);
}

void MainWindow::on_radioButton_2rainbow_page1_clicked()
{
    scene1->setDisplayMode(2);
}

void MainWindow::on_pushButton_beamPrev_page1_clicked()
{
    if (!scene1->ifNoBeams())
        scene1->decBeamStep();
}

void MainWindow::on_pushButton_beamNext_page1_clicked()
{
    if (!scene1->ifNoBeams())
        scene1->incBeamStep();
}

void MainWindow::on_pushButton_addBeam_page1_clicked()
{
    double d = ui->doubleSpinBox_dist_page1->value();
    int wl = ui->spinBox_wave_page1->value();
    scene1->addBeam(d, wl);
}

void MainWindow::on_pushButton_clear_screen_page1_clicked()
{
    scene1->clearBeams();
}