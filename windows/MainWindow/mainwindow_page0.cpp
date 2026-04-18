#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton_rainbow_page0_clicked()
{
    currentSlideWidgetPage = 0;
    switchSlide();
}

void MainWindow::on_radioButton_refraction_page0_clicked()
{
    currentSlideWidgetPage = 1;
    switchSlide();
}

void MainWindow::on_radioButton_1rainbow_page0_clicked()
{
    currentSlideWidgetPage = 2;
    switchSlide();
}

void MainWindow::on_radioButton_2rainbow_page0_clicked()
{
    currentSlideWidgetPage = 3;
    switchSlide();
}