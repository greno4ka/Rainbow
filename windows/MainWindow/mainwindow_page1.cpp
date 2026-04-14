#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_doubleSpinBox_dist_page1_valueChanged(double value)
{
    scene1->setDistance(value);
    ui->horizontalSlider_dist_page1->setValue(value*100);
}

void MainWindow::on_horizontalSlider_dist_page1_valueChanged(int value)
{
    scene1->setDistance((double)value/100);
    ui->doubleSpinBox_dist_page1->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page1_valueChanged(int value)
{
    scene1->setWavelength(value);
    ui->horizontalSlider_wave_page1->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page1_valueChanged(int value)
{
    scene1->setWavelength(value);
    ui->spinBox_wave_page1->setValue(value);
}

void MainWindow::on_radioButton0_page1_clicked()
{
    scene1->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page1_clicked()
{
    scene1->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page1_clicked()
{
    scene1->setDisplayMode(2);
}

void MainWindow::on_pushButton_addBeam_page1_clicked()
{
    double d = ui->doubleSpinBox_dist_page1->value();
    int wl = ui->spinBox_wave_page1->value();
    scene1->addBeam(d, wl);
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

void MainWindow::on_pushButton_clearScr_page1_clicked()
{
    scene1->clearBeams();
}