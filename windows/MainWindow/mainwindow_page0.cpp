#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton_rainbow_page0_clicked()
{
    currentSlide = 0;
    currentSlideName = "rainbow";
    ui->presentationWidget->setCurrentIndex(0);
    updateSlide();
}

void MainWindow::on_radioButton_refraction_page0_clicked()
{
    currentSlide = 1;
    currentSlideName = "slide2";
    ui->presentationWidget->setCurrentIndex(1);
    updateSlide();
}

void MainWindow::on_radioButton_1rainbow_page0_clicked()
{
    currentSlide = 2;
    currentSlideName = "slide3";
    ui->presentationWidget->setCurrentIndex(2);
    updateSlide();
}

void MainWindow::on_radioButton_2rainbow_page0_clicked()
{
    currentSlide = 3;
    currentSlideName = "slide4";
    ui->presentationWidget->setCurrentIndex(3);
    updateSlide();
}