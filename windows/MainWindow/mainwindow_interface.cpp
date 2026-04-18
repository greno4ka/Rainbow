#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_next_clicked()
{
    if (++currentMenuWidgetPage == ui->menuWidget->count())
        currentMenuWidgetPage = 0;
    switchPage();
}

void MainWindow::on_pushButton_prev_clicked()
{
    if (--currentMenuWidgetPage < 0)
        currentMenuWidgetPage = ui->menuWidget->count() - 1;
    switchPage();
}

void MainWindow::on_pushButton_fullscreen_clicked()
{
    isFullScreen() ? showNormal() : showFullScreen();
}

void MainWindow::on_pushButton_close_clicked()
{
    close();
}

void MainWindow::on_pushButton_settings_clicked()
{
    settingsWindow->show();
}