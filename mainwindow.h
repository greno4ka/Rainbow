#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "settingswindow.h"
#include "glwidget.h"
#include "scenes/scenebase.h"
#include "scenes/scene0.h"
#include "scenes/scene1.h"
#include "scenes/scene2.h"
#include "scenes/scene3.h"
#include "scenes/scene4.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int programMode, QWidget *parent = nullptr);
    ~MainWindow();

    void setTranslator(QTranslator *newTranslator);

private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
    GLWidget *glWidget;

    SceneBase *scene;
    Scene0 *scene0;
    Scene1 *scene1;
    Scene2 *scene2;
    Scene3 *scene3;
    Scene4 *scene4;

    QTranslator *translator = nullptr;

    int currentStackWidgetPage;

    void switchScene();

private slots:
    void retranslate();
    /// Main controls
    void on_pushButton_next_clicked();
    void on_pushButton_prev_clicked();
    void on_pushButton_fullscreen_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_settings_clicked();

    /// page1
    void on_doubleSpinBox_dist_page1_valueChanged(double arg1);
    void on_horizontalSlider_dist_page1_valueChanged(int value);
    void on_spinBox_wave_page1_valueChanged(int arg1);
    void on_horizontalSlider_wave_page1_valueChanged(int value);
    void on_radioButton0_page1_clicked();
    void on_radioButton1_page1_clicked();
    void on_radioButton2_page1_clicked();
    void on_pushButton_beamPrev_page1_clicked();
    void on_pushButton_beamNext_page1_clicked();
    void on_pushButton_addBeam_page1_clicked();
    void on_pushButton_clearScr_page1_clicked();
    /// page2
    void on_doubleSpinBox_dist_page2_valueChanged(double value);
    void on_horizontalSlider_dist_page2_valueChanged(int value);
    void on_spinBox_wave_page2_valueChanged(int value);
    void on_horizontalSlider_wave_page2_valueChanged(int value);
    void on_radioButton1_page2_clicked();
    void on_radioButton2_page2_clicked();
    void on_pushButton_clearScr_page2_clicked();
    /// page3
    void on_spinBox_wave_page3_valueChanged(int value);
    void on_horizontalSlider_wave_page3_valueChanged(int value);
    void on_spinBox_number_of_beams_page3_valueChanged(int value);
    void on_radioButton1_page3_clicked();
    void on_radioButton2_page3_clicked();
    void on_checkBox_show_angle_page3_stateChanged(int value);
    void on_checkBox_show_rainbow_beams_page3_stateChanged(int value);
    void on_horizontalSlider_number_of_beams_page3_valueChanged(int value);
    void on_pushButton_reset_settings_page3_clicked();
    /// page4
    void on_radioButton0_page4_clicked();
    void on_radioButton1_page4_clicked();
    void on_radioButton2_page4_clicked();
    void on_pushButton_startpause_page4_clicked();
    void on_spinBox_FPS_page4_valueChanged(int value);
    void on_horizontalSlider_FPS_page4_valueChanged(int value);
    /// page5
    void on_radioButton0_page5_clicked();
    void on_radioButton1_page5_clicked();
    void on_radioButton2_page5_clicked();
};

#endif // MAINWINDOW_H
