#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton_rainbow_page0_clicked()
{
    currentSlideWidgetPage = 0;
    currentSlideName = "rainbow";
    ui->slideWidget->setCurrentIndex(0);
    updateSlide();
}

void MainWindow::on_radioButton_refraction_page0_clicked()
{
    currentSlideWidgetPage = 1;
    currentSlideName = "slide1";
    ui->slideWidget->setCurrentIndex(1);
    updateSlide();
}

void MainWindow::on_radioButton_1rainbow_page0_clicked()
{
    currentSlideWidgetPage = 2;
    currentSlideName = "slide2";
    ui->slideWidget->setCurrentIndex(2);
    updateSlide();
}

void MainWindow::on_radioButton_2rainbow_page0_clicked()
{
    currentSlideWidgetPage = 3;
    currentSlideName = "slide3";
    ui->slideWidget->setCurrentIndex(3);
    updateSlide();
}