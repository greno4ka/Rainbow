#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "scenes/scene0.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Scene0 *scene0;

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
    void on_horizontalSlider_dist_page0_valueChanged(int value);
    void on_horizontalSlider_wave_page0_valueChanged(int value);
    void on_doubleSpinBox_dist_page0_valueChanged(double arg1);
    void on_spinBox_wave_page0_valueChanged(int arg1);
    void on_radioButton0_page0_clicked();
    void on_radioButton1_page0_clicked();
    void on_radioButton2_page0_clicked();
    void on_pushButton_beamPrev_page0_clicked();
    void on_pushButton_beamNext_page0_clicked();
    void on_pushButton_addBeam_page0_clicked();
    void on_pushButton_clearScr_page0_clicked();
};

#endif // MAINWINDOW_H
