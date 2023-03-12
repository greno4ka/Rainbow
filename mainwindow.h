#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "scenes/scene0.h"
#include "scenes/scene1.h"
#include "scenes/scene2.h"

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
    Scene1 *scene1;
    Scene2 *scene2;

private:
    Ui::MainWindow *ui;

    int currentStackWidgetPage;

private slots:
    /// Main controls
    void on_pushButton_next_clicked();
    void on_pushButton_prev_clicked();
    void on_pushButton_fullscreen_clicked();
    void on_pushButton_close_clicked();

    /// PAGE0
    void on_doubleSpinBox_dist_page0_valueChanged(double arg1);
    void on_horizontalSlider_dist_page0_valueChanged(int value);
    void on_spinBox_wave_page0_valueChanged(int arg1);
    void on_horizontalSlider_wave_page0_valueChanged(int value);
    void on_radioButton0_page0_clicked();
    void on_radioButton1_page0_clicked();
    void on_radioButton2_page0_clicked();
    void on_pushButton_beamPrev_page0_clicked();
    void on_pushButton_beamNext_page0_clicked();
    void on_pushButton_addBeam_page0_clicked();
    void on_pushButton_clearScr_page0_clicked();
    /// PAGE1
    void on_doubleSpinBox_dist_page1_valueChanged(double value);
    void on_horizontalSlider_dist_page1_valueChanged(int value);
    void on_spinBox_wave_page1_valueChanged(int value);
    void on_horizontalSlider_wave_page1_valueChanged(int value);
    void on_radioButton1_page1_clicked();
    void on_radioButton2_page1_clicked();
    void on_pushButton_clearScr_page1_clicked();
    /// PAGE2
    void on_spinBox_wave_page2_valueChanged(int value);
    void on_horizontalSlider_wave_page2_valueChanged(int value);
    void on_spinBox_number_of_beams_page2_valueChanged(int value);
    void on_radioButton1_page2_clicked();
    void on_radioButton2_page2_clicked();
    void on_checkBox_show_angle_page2_stateChanged(int value);
    void on_checkBox_show_rainbow_beams_page2_stateChanged(int value);
    void on_pushButton_reset_settings_page2_clicked();
    /// PAGE3
    void on_radioButton0_page3_clicked();
    void on_radioButton1_page3_clicked();
    void on_radioButton2_page3_clicked();
    void on_pushButton_startpause_page3_clicked();
    void on_horizontalSlider_number_of_beams_page2_valueChanged(int value);
};

#endif // MAINWINDOW_H
