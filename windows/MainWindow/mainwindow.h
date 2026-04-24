#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QSurfaceFormat>
#include <QStandardPaths>
#include <QSettings>
#include <QResizeEvent>
#include <QMessageBox>

#include "settingswindow.h"
#include "glwidget.h"
#include "glwidget3d.h"
#include "scenes/scenebase.h"
#include "scenes/scene1.h"
#include "scenes/scene2.h"
#include "scenes/scene3.h"
#include "scenes/scene4.h"
#include "scenes/scene5.h"
#include "scenes/scene6.h"
#include "scenes/scenex.h"

#include "plotFactory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int programMode, QTranslator *newTranslator, QWidget *parent = nullptr);
    ~MainWindow();

    void setTranslator(QTranslator *newTranslator);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui = nullptr;
    SettingsWindow *settingsWindow = nullptr;

    GLWidget *glWidget = nullptr;
    GLWidget3D *glWidget3d = nullptr;

    QwtPlot *refractiveIndexPlot = nullptr;
    QwtPlot *primaryRainbowPlot = nullptr;
    QwtPlot *secondaryRainbowPlot = nullptr;

    SceneBase *scene = nullptr;
    Scene1 *scene1 = nullptr;
    Scene2 *scene2 = nullptr;
    Scene3 *scene3 = nullptr;
    Scene4 *scene4 = nullptr;
    Scene5 *scene5 = nullptr;
    SceneX *scenex = nullptr;

    Scene6 *scene6 = nullptr;
    Scene7 *scene7 = nullptr;

    QTranslator *translator = nullptr;

    int currentMenuWidgetPage;
    int currentSlideWidgetPage;

    QString configPath;
    QString settingsFilePath;
    QSettings *settings = nullptr;

    QString appLanguage;
    bool multisamplingEnabled;
    bool fullscreenEnabled;
    bool darkThemeEnabled;

    QPixmap rainbowPixmap;

    void updateRainbowImage();
    void switchPage();
    void switchSlide();

    void initUIDefaults();
    void reInitializePlots();

    void mathToLabel(QLabel* label,
                     const QString& formula,
                     int width = 650,
                     int height = 100,
                     int fontSize = 20,
                     const QColor& color = Qt::white);

private slots:
    /// changers
    void changeLanguage(const QString &language);
    void changeTheme(bool isDark);
    void changeMultisampling(bool enabled);
    void changeFullscreen(bool enabled);

    /// interface
    void on_pushButton_next_clicked();
    void on_pushButton_prev_clicked();
    void on_pushButton_fullscreen_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_settings_clicked();

    /// page0
    void on_radioButton_rainbow_page0_clicked();
    void on_radioButton_refraction_page0_clicked();
    void on_radioButton_1rainbow_page0_clicked();
    void on_radioButton_2rainbow_page0_clicked();

    /// page1
    void on_doubleSpinBox_dist_page1_valueChanged(double value);
    void on_horizontalSlider_dist_page1_valueChanged(int value);
    void on_spinBox_wave_page1_valueChanged(int value);
    void on_horizontalSlider_wave_page1_valueChanged(int value);
    void on_radioButton_all_beams_page1_clicked();
    void on_radioButton_1rainbow_page1_clicked();
    void on_radioButton_2rainbow_page1_clicked();
    void on_pushButton_beamPrev_page1_clicked();
    void on_pushButton_beamNext_page1_clicked();
    void on_pushButton_addBeam_page1_clicked();
    void on_pushButton_clear_screen_page1_clicked();

    /// page2
    void on_doubleSpinBox_dist_page2_valueChanged(double value);
    void on_horizontalSlider_dist_page2_valueChanged(int value);
    void on_spinBox_wave_page2_valueChanged(int value);
    void on_horizontalSlider_wave_page2_valueChanged(int value);
    void on_radioButton_1rainbow_page2_clicked();
    void on_radioButton_2rainbow_page2_clicked();
    void on_pushButton_reset_settings_page2_clicked();

    /// page3
    void on_spinBox_wave_page3_valueChanged(int value);
    void on_horizontalSlider_wave_page3_valueChanged(int value);
    void on_spinBox_number_of_beams_page3_valueChanged(int value);
    void on_radioButton_1rainbow_page3_clicked();
    void on_radioButton_2rainbow_page3_clicked();
    void on_checkBox_show_angle_page3_stateChanged(int value);
    void on_checkBox_show_rainbow_beams_page3_stateChanged(int value);
    void on_horizontalSlider_number_of_beams_page3_valueChanged(int value);
    void on_pushButton_reset_settings_page3_clicked();

    /// page4
    void on_radioButton_all_beams_page4_clicked();
    void on_radioButton_observed_beams_page4_clicked();
    void on_radioButton_regions_beams_page4_clicked();
    void on_pushButton_startpause_page4_clicked();
    void on_spinBox_FPS_page4_valueChanged(int value);
    void on_horizontalSlider_FPS_page4_valueChanged(int value);

    /// page5
    void on_spinBox_number_of_beams_page5_valueChanged(int value);
    void on_horizontalSlider_number_of_beams_page5_valueChanged(int value);
    void on_spinBox_dispersion_quality_page5_valueChanged(int value);
    void on_horizontalSlider_dispersion_quality_page5_valueChanged(int value);
    void on_radioButton_both_rainbows_page5_clicked();
    void on_radioButton_1rainbow_page5_clicked();
    void on_radioButton_2rainbow_page5_clicked();
    void on_pushButton_reset_settings_page5_clicked();

    /// page6
    void on_verticalSlider_sun_position_page6_valueChanged(int value);
    void on_verticalSlider_eye_position_page6_valueChanged(int value);
    void on_radioButton_polychromatic_page6_clicked();
    void on_radioButton_monochromatic_page6_clicked();
    void on_checkBox_show_beams_page6_stateChanged(int value);
    void on_spinBox_number_of_beams_page6_valueChanged(int value);
    void on_pushButton_add_beams_page6_clicked();
    void on_pushButton_reset_settings_page6_clicked();
    void on_radioButton_fly_to_eye_page6_clicked();
    void on_radioButton_back_to_god_page6_clicked();
    void on_pushButton_reset_camera_page6_clicked();
};

#endif // MAINWINDOW_H
