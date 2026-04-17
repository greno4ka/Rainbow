#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton_rainbow_page0_clicked()
{
    currentSlide = "rainbow";
    updateSlide();
}

void MainWindow::on_radioButton_refraction_page0_clicked()
{
    currentSlide = "slide2";
    updateSlide();
}

void MainWindow::on_radioButton_1rainbow_page0_clicked()
{
    currentSlide = "slide3";
    updateSlide();
}

void MainWindow::on_radioButton_2rainbow_page0_clicked()
{
    currentSlide = "slide4";
    updateSlide();
}