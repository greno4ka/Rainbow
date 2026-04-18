#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_radioButton_rainbow_page0_clicked()
{
    currentSlideWidgetPage = 0;
    ui->slideWidget->setCurrentIndex(currentSlideWidgetPage);
}

void MainWindow::on_radioButton_refraction_page0_clicked()
{
    currentSlideWidgetPage = 1;
    ui->slideWidget->setCurrentIndex(currentSlideWidgetPage);
    ui->glWidgetLayout_slide1->addWidget(glWidget);
    glWidget->setSceneNumber(6);

}

void MainWindow::on_radioButton_1rainbow_page0_clicked()
{
    currentSlideWidgetPage = 2;
    ui->slideWidget->setCurrentIndex(currentSlideWidgetPage);
    ui->glWidgetLayout_slide2->addWidget(glWidget);
    glWidget->setSceneNumber(7);
    scene7->setDisplayMode(0);
}

void MainWindow::on_radioButton_2rainbow_page0_clicked()
{
    currentSlideWidgetPage = 3;
    ui->slideWidget->setCurrentIndex(currentSlideWidgetPage);
    ui->glWidgetLayout_slide3->addWidget(glWidget);
    glWidget->setSceneNumber(7);
    scene7->setDisplayMode(1);
}