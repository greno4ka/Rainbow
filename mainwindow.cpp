#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QSurfaceFormat>

#include "glwidget.h"

#include "settingswindow.h"

MainWindow::MainWindow(int programMode, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settingsWindow = new SettingsWindow(this);
    connect(settingsWindow, SIGNAL(language_change()), this, SLOT(retranslate()));
    connect(settingsWindow, SIGNAL(theme_change(bool)), this, SLOT(onThemeChanged(bool)));
    connect(settingsWindow, SIGNAL(multisampling_change(bool)), this, SLOT(onMultisamplingChanged(bool)));
    connect(settingsWindow, SIGNAL(fullscreen_change(bool)), this, SLOT(onFullscreenChanged(bool)));

    glWidget = new GLWidget(this);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    glWidget->setSizePolicy(sizePolicy);

    // Initial format will be set by the settings loaded in SettingsWindow constructor
    ui->glWidgetLayout->addWidget(glWidget);
    currentStackWidgetPage = programMode;
    scene1 = new Scene1();
    scene2 = new Scene2();
    scene3 = new Scene3();
    scene4 = new Scene4();
    scene5 = new Scene5();

    glWidget->connectWithScene1(*scene1);
    glWidget->connectWithScene2(*scene2);
    glWidget->connectWithScene3(*scene3);
    glWidget->connectWithScene4(*scene4);
    glWidget->connectWithScene5(*scene5);
    switchScene();
}

void MainWindow::retranslate()
{
    ui->retranslateUi(this);
}

MainWindow::~MainWindow()
{
    delete scene1;
    delete scene2;
    delete scene3;
    delete scene4;
    delete scene5;

    delete translator;
    delete settingsWindow;

    delete ui;
}

void MainWindow::setTranslator(QTranslator *newTranslator)
{
    translator = newTranslator;
}

void MainWindow::switchWidget()
{
    switch (currentStackWidgetPage) {
    case 0:
        glWidget->hide();
        ui->presentationWidget->show();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        glWidget->show();
        ui->presentationWidget->hide();
        break;
    }
}

void MainWindow::switchScene()
{
    switch (currentStackWidgetPage) {
    case 0:
        ui->label_pageName->setText(tr("Analytical calculations"));
        break;
    case 1:
        ui->label_pageName->setText(tr("Beams in a droplet"));
        break;
    case 2:
        ui->label_pageName->setText(tr("Beam divergence"));
        break;
    case 3:
        ui->label_pageName->setText(tr("Descartes' rainbow"));
        break;
    case 4:
        ui->label_pageName->setText(tr("Ensemble of droplets"));
        break;
    case 5:
        ui->label_pageName->setText(tr("Alexander's dark band"));
        break;
    default:
        break;
    }

    ui->stackedWidget->setCurrentIndex(currentStackWidgetPage);
    glWidget->setSceneNumber(currentStackWidgetPage);
    switchWidget();
}

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

void MainWindow::on_doubleSpinBox_dist_page1_valueChanged(double value)
{
    scene1->setDistance(value);
    ui->horizontalSlider_dist_page1->setValue(value*100);
}

void MainWindow::on_horizontalSlider_dist_page1_valueChanged(int value)
{
    scene1->setDistance((double)value/100);
    ui->doubleSpinBox_dist_page1->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page1_valueChanged(int value)
{
    scene1->setWavelength(value);
    ui->horizontalSlider_wave_page1->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page1_valueChanged(int value)
{
    scene1->setWavelength(value);
    ui->spinBox_wave_page1->setValue(value);
}

void MainWindow::on_radioButton0_page1_clicked()
{
    scene1->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page1_clicked()
{
    scene1->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page1_clicked()
{
    scene1->setDisplayMode(2);
}

void MainWindow::on_pushButton_addBeam_page1_clicked()
{
    double d = ui->doubleSpinBox_dist_page1->value();
    int wl = ui->spinBox_wave_page1->value();
    scene1->addBeam(d, wl);
}

void MainWindow::on_pushButton_beamPrev_page1_clicked()
{
    if (!scene1->ifNoBeams())
        scene1->decBeamStep();
}

void MainWindow::on_pushButton_beamNext_page1_clicked()
{
    if (!scene1->ifNoBeams())
        scene1->incBeamStep();
}

void MainWindow::on_pushButton_clearScr_page1_clicked()
{
    scene1->clearBeams();
}

void MainWindow::on_doubleSpinBox_dist_page2_valueChanged(double value)
{
    scene2->setDistance(value);
    ui->horizontalSlider_dist_page2->setValue(value*100);
}


void MainWindow::on_horizontalSlider_dist_page2_valueChanged(int value)
{
    scene2->setDistance((double)value/100);
    ui->doubleSpinBox_dist_page2->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page2_valueChanged(int value)
{
    scene2->setWavelength(value);
    ui->horizontalSlider_wave_page2->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page2_valueChanged(int value)
{
    scene2->setWavelength(value);
    ui->spinBox_wave_page2->setValue(value);
}


void MainWindow::on_radioButton1_page2_clicked()
{
    scene2->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page2_clicked()
{
    scene2->setDisplayMode(2);
}

void MainWindow::on_pushButton_clearScr_page2_clicked()
{
    double d=0.86; // good inception with parallel beams
    int wl=600;
    ui->doubleSpinBox_dist_page2->setValue(d);
    ui->horizontalSlider_dist_page2->setValue(d*100);
    ui->spinBox_wave_page2->setValue(wl);
    ui->horizontalSlider_wave_page2->setValue(wl);
    ui->radioButton1_page2->setChecked(1);
    scene2->setDisplayMode(1);
}

void MainWindow::on_spinBox_wave_page3_valueChanged(int value)
{
    scene3->setWavelength(value);
    ui->horizontalSlider_wave_page3->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page3_valueChanged(int value)
{
    scene3->setWavelength(value);
    ui->spinBox_wave_page3->setValue(value);
}

void MainWindow::on_spinBox_number_of_beams_page3_valueChanged(int value)
{
    scene3->setNumberOfBeams(ui->spinBox_number_of_beams_page3->value());
    ui->horizontalSlider_number_of_beams_page3->setValue(value);
}

void MainWindow::on_horizontalSlider_number_of_beams_page3_valueChanged(int value)
{
    scene3->setNumberOfBeams(ui->spinBox_number_of_beams_page3->value());
    ui->spinBox_number_of_beams_page3->setValue(value);
}

void MainWindow::on_radioButton1_page3_clicked()
{
    scene3->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page3_clicked()
{
    scene3->setDisplayMode(2);
}

void MainWindow::on_checkBox_show_angle_page3_stateChanged(int value)
{
    scene3->setShowAngle(value);
}

void MainWindow::on_checkBox_show_rainbow_beams_page3_stateChanged(int value)
{
    scene3->setShowRainbow(value);
}

void MainWindow::on_pushButton_reset_settings_page3_clicked()
{
    scene3->resetScene();
    ui->spinBox_wave_page3->setValue(600);
    ui->horizontalSlider_wave_page3->setValue(600);
    ui->spinBox_number_of_beams_page3->setValue(30);
    ui->horizontalSlider_number_of_beams_page3->setValue(30);
    ui->radioButton1_page3->setChecked(1);
    ui->checkBox_show_angle_page3->setChecked(0);
    ui->checkBox_show_rainbow_beams_page3->setChecked(0);
}

void MainWindow::on_radioButton0_page4_clicked()
{
    scene4->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page4_clicked()
{
    scene4->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page4_clicked()
{
    scene4->setDisplayMode(2);
}

void MainWindow::on_pushButton_startpause_page4_clicked()
{
    scene4->switchDynamicMode();
    if (scene4->getDynamicMode())
        ui->pushButton_startpause_page4->setText(tr("Pause"));
    else
        ui->pushButton_startpause_page4->setText(tr("Start"));
}

void MainWindow::on_spinBox_FPS_page4_valueChanged(int value)
{
    ui->horizontalSlider_FPS_page4->setValue(value);
    scene4->setDesiredFPS(value);
}

void MainWindow::on_horizontalSlider_FPS_page4_valueChanged(int value)
{
    ui->spinBox_FPS_page4->setValue(value);
    scene4->setDesiredFPS(value);
}

void MainWindow::on_radioButton0_page5_clicked()
{
    scene5->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page5_clicked()
{
    scene5->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page5_clicked()
{
    scene5->setDisplayMode(2);
}

void MainWindow::onThemeChanged(bool isDark)
{
    applyTheme(isDark);
}

void MainWindow::applyTheme(bool isDark)
{
    // Create the base stylesheet
    QString baseStyle;
    if (isDark) {
        baseStyle = "QWidget { background-color: #2b2b2b; color: #ffffff; }"
                   "QPushButton { background-color: #3b3b3b; border: 1px solid #555555; padding: 5px; }"
                   "QPushButton:hover { background-color: #4b4b4b; }"
                   "QComboBox { background-color: #3b3b3b; border: 1px solid #555555; padding: 5px; }"
                   "QSpinBox, QDoubleSpinBox { background-color: #3b3b3b; border: 1px solid #555555; padding: 5px; }"
                   "QSlider::handle:horizontal { background-color: #555555; }"
                   "QSlider::groove:horizontal { background-color: #3b3b3b; }"
                   "QCheckBox { color: #ffffff; }"
                   "QRadioButton { color: #ffffff; }"
                   "QGroupBox { border: 1px solid #555555; margin-top: 5px; }"
                   "QGroupBox::title { color: #ffffff; }";
    } else {
        baseStyle = "QWidget { background-color: #f0f0f0; color: #000000; }"
                   "QPushButton { background-color: #e0e0e0; border: 1px solid #c0c0c0; padding: 5px; }"
                   "QPushButton:hover { background-color: #d0d0d0; }"
                   "QComboBox { background-color: #ffffff; border: 1px solid #c0c0c0; padding: 5px; }"
                   "QSpinBox, QDoubleSpinBox { background-color: #ffffff; border: 1px solid #c0c0c0; padding: 5px; }"
                   "QSlider::handle:horizontal { background-color: #c0c0c0; }"
                   "QSlider::groove:horizontal { background-color: #e0e0e0; }"
                   "QCheckBox { color: #000000; }"
                   "QRadioButton { color: #000000; }"
                   "QGroupBox { border: 1px solid #c0c0c0; margin-top: 5px; }"
                   "QGroupBox::title { color: #000000; }";
    }

    // Apply the stylesheet to both windows
    this->setStyleSheet(baseStyle);
    settingsWindow->setStyleSheet(baseStyle);

    // Update GL widget background color
    if (glWidget) {
        glWidget->makeCurrent();
        if (isDark) {
            glClearColor(0.17f, 0.17f, 0.17f, 1.0f);  // Dark gray for dark theme
        } else {
            glClearColor(0.94f, 0.94f, 0.94f, 1.0f);  // Light gray for light theme
        }
        glWidget->update();
    }
}

void MainWindow::applyMultisampling(bool enabled)
{
    if (glWidget) {
        QSurfaceFormat format = glWidget->format();
        format.setSamples(enabled ? 8 : 0);
        glWidget->setFormat(format);
        glWidget->hide();  // Force a context recreation
        glWidget->show();
    }
}

void MainWindow::onMultisamplingChanged(bool enabled)
{
    applyMultisampling(enabled);
}

void MainWindow::onFullscreenChanged(bool enabled)
{
    if (enabled) {
        showFullScreen();
    } else {
        showNormal();
    }
}
