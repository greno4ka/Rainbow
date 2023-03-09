#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int currentStackWidgetPage;

private slots:
    /// Main controls
    void on_pushButton_next_clicked();
    void on_pushButton_prev_clicked();
    void on_pushButton_fullscreen_clicked();
    void on_pushButton_close_clicked();


    void on_radioButton0_page3_clicked();
    void on_radioButton1_page3_clicked();
    void on_radioButton2_page3_clicked();
    void on_pushButton_startpause_page3_clicked();
};

#endif // MAINWINDOW_H
