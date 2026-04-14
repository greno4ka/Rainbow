#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_next_clicked()
{
    if (++currentStackWidgetPage == ui->stackedWidget->count())
        currentStackWidgetPage = 0;
    switchScene();
}

void MainWindow::on_pushButton_prev_clicked()
{
    if (--currentStackWidgetPage < 0)
        currentStackWidgetPage = ui->stackedWidget->count() - 1;
    switchScene();
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
    settingsWindow->setTranslator(translator);
    settingsWindow->show();
}