#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton_all_beams_page4_clicked()
{
    scene4->setDisplayMode(0);
}

void MainWindow::on_radioButton_observed_beams_page4_clicked()
{
    scene4->setDisplayMode(1);
}

void MainWindow::on_radioButton_regions_beams_page4_clicked()
{
    scene4->setDisplayMode(2);
}

void MainWindow::on_pushButton_startpause_page4_clicked()
{
    scene4->switchDynamicMode();
    if (scene4->getDynamicMode())
        ui->pushButton_startpause_page4->setText(tr("Pause"));
    else
        ui->pushButton_startpause_page4->setText(tr("Start"));
}

void MainWindow::on_spinBox_FPS_page4_valueChanged(int value)
{
    ui->horizontalSlider_FPS_page4->setValue(value);
    scene4->setDesiredFPS(value);
}

void MainWindow::on_horizontalSlider_FPS_page4_valueChanged(int value)
{
    ui->spinBox_FPS_page4->setValue(value);
    scene4->setDesiredFPS(value);
}