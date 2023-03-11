#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentStackWidgetPage = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_next_clicked()
{
    if (++currentStackWidgetPage == ui->stackedWidget->count())
        currentStackWidgetPage = 0;
    ui->stackedWidget->setCurrentIndex(currentStackWidgetPage);
}

void MainWindow::on_pushButton_prev_clicked()
{
    if (--currentStackWidgetPage < 0)
        currentStackWidgetPage = ui->stackedWidget->count() - 1;
    ui->stackedWidget->setCurrentIndex(currentStackWidgetPage);
}

void MainWindow::on_pushButton_fullscreen_clicked()
{
    isFullScreen() ? showNormal() : showFullScreen();
}

void MainWindow::on_pushButton_close_clicked()
{
    close();
}

void MainWindow::on_radioButton0_page3_clicked()
{

}

void MainWindow::on_radioButton1_page3_clicked()
{

}

void MainWindow::on_radioButton2_page3_clicked()
{

}

void MainWindow::on_pushButton_startpause_page3_clicked()
{

}

void MainWindow::on_horizontalSlider_dist_page0_valueChanged(int value)
{
    scene0.setDistance((double)value/100);
    ui->doubleSpinBox_dist_page0->setValue((double)value/100);
}

void MainWindow::on_horizontalSlider_wave_page0_valueChanged(int value)
{
    scene0.setWavelength(value);
    ui->spinBox_wave_page0->setValue(value);
}

void MainWindow::on_doubleSpinBox_dist_page0_valueChanged(double value)
{
    scene0.setDistance(value);
    ui->horizontalSlider_dist_page0->setValue(value*100);
}

void MainWindow::on_spinBox_wave_page0_valueChanged(int value)
{
    scene0.setWavelength(value);
    ui->horizontalSlider_wave_page0->setValue(value);
}
