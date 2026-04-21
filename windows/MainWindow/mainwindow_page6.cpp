#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_verticalSlider_sun_position_page6_valueChanged(int value)
{
    scenex->setSunHeight(value);
}

void MainWindow::on_verticalSlider_eye_position_page6_valueChanged(int value)
{
    scenex->setEyeHeight(value);
}

void MainWindow::on_radioButton_polychromatic_page6_clicked()
{
    scenex->setPolychromatic(1);
}

void MainWindow::on_radioButton_monochromatic_page6_clicked()
{
    scenex->setPolychromatic(0);
}

void MainWindow::on_checkBox_show_beams_page6_stateChanged(int value)
{
    scenex->setShowBeams(value);
}

void MainWindow::on_spinBox_number_of_beams_page6_valueChanged(int value)
{

}

void MainWindow::on_pushButton_add_beams_page6_clicked()
{
    scenex->addSunPoints(ui->spinBox_number_of_beams_page6->value());
    scenex->addNumberOfBeams(ui->spinBox_number_of_beams_page6->value());
}

void MainWindow::on_pushButton_reset_settings_page6_clicked()
{
    scenex->resetScene();
}