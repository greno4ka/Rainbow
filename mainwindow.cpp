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

    glWidget = new GLWidget(this);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    glWidget->setSizePolicy(sizePolicy);

    QSurfaceFormat format;
    format.setSamples(8); //this creates a 8x MSAA surface format
    glWidget->setFormat(format);
    ui->glWidgetLayout->addWidget(glWidget);
    currentStackWidgetPage = programMode;
    scene0 = new Scene0();
    scene1 = new Scene1();
    scene2 = new Scene2();
    scene3 = new Scene3();
    scene4 = new Scene4();

    glWidget->connectWithScene0(*scene0);
    glWidget->connectWithScene1(*scene1);
    glWidget->connectWithScene2(*scene2);
    glWidget->connectWithScene3(*scene3);
    glWidget->connectWithScene4(*scene4);
    switchScene();
}

void MainWindow::retranslate()
{
    ui->retranslateUi(this);
}

MainWindow::~MainWindow()
{
    delete scene0;
    delete scene1;
    delete scene2;
    delete scene3;
    delete scene4;

    delete translator;
    delete settingsWindow;

    delete ui;
}

void MainWindow::setTranslator(QTranslator *newTranslator)
{
    translator = newTranslator;
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
    scene0->setDistance(value);
    ui->horizontalSlider_dist_page1->setValue(value*100);
}

void MainWindow::on_horizontalSlider_dist_page1_valueChanged(int value)
{
    scene0->setDistance((double)value/100);
    ui->doubleSpinBox_dist_page1->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page1_valueChanged(int value)
{
    scene0->setWavelength(value);
    ui->horizontalSlider_wave_page1->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page1_valueChanged(int value)
{
    scene0->setWavelength(value);
    ui->spinBox_wave_page1->setValue(value);
}

void MainWindow::on_radioButton0_page1_clicked()
{
    scene0->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page1_clicked()
{
    scene0->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page1_clicked()
{
    scene0->setDisplayMode(2);
}

void MainWindow::on_pushButton_addBeam_page1_clicked()
{
    double d = ui->doubleSpinBox_dist_page1->value();
    int wl = ui->spinBox_wave_page1->value();
    scene0->addBeam(d, wl);
}

void MainWindow::on_pushButton_beamPrev_page1_clicked()
{
    if (!scene0->ifNoBeams())
        scene0->decBeamStep();
}

void MainWindow::on_pushButton_beamNext_page1_clicked()
{
    if (!scene0->ifNoBeams())
        scene0->incBeamStep();
}

void MainWindow::on_pushButton_clearScr_page1_clicked()
{
    scene0->resetBeamStep();
}

void MainWindow::on_doubleSpinBox_dist_page2_valueChanged(double value)
{
    scene1->setDistance(value);
    ui->horizontalSlider_dist_page2->setValue(value*100);
}


void MainWindow::on_horizontalSlider_dist_page2_valueChanged(int value)
{
    scene1->setDistance((double)value/100);
    ui->doubleSpinBox_dist_page2->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page2_valueChanged(int value)
{
    scene1->setWavelength(value);
    ui->horizontalSlider_wave_page2->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page2_valueChanged(int value)
{
    scene1->setWavelength(value);
    ui->spinBox_wave_page2->setValue(value);
}


void MainWindow::on_radioButton1_page2_clicked()
{
    scene1->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page2_clicked()
{
    scene1->setDisplayMode(2);
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
    scene1->setDisplayMode(1);
}

void MainWindow::on_spinBox_wave_page3_valueChanged(int value)
{
    scene2->setWavelength(value);
    ui->horizontalSlider_wave_page3->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page3_valueChanged(int value)
{
    scene2->setWavelength(value);
    ui->spinBox_wave_page3->setValue(value);
}

void MainWindow::on_spinBox_number_of_beams_page3_valueChanged(int value)
{
    scene2->setNumberOfBeams(ui->spinBox_number_of_beams_page3->value());
    ui->horizontalSlider_number_of_beams_page3->setValue(value);
}

void MainWindow::on_horizontalSlider_number_of_beams_page3_valueChanged(int value)
{
    scene2->setNumberOfBeams(ui->spinBox_number_of_beams_page3->value());
    ui->spinBox_number_of_beams_page3->setValue(value);
}

void MainWindow::on_radioButton1_page3_clicked()
{
    scene2->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page3_clicked()
{
    scene2->setDisplayMode(2);
}

void MainWindow::on_checkBox_show_angle_page3_stateChanged(int value)
{
    scene2->setShowAngle(value);
}

void MainWindow::on_checkBox_show_rainbow_beams_page3_stateChanged(int value)
{
    scene2->setShowRainbow(value);
}

void MainWindow::on_pushButton_reset_settings_page3_clicked()
{
    scene2->resetScene();
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
    scene3->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page4_clicked()
{
    scene3->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page4_clicked()
{
    scene3->setDisplayMode(2);
}

void MainWindow::on_pushButton_startpause_page4_clicked()
{
    scene3->switchDynamicMode();
    if (scene3->getDynamicMode())
        ui->pushButton_startpause_page4->setText(tr("Pause"));
    else
        ui->pushButton_startpause_page4->setText(tr("Start"));
}

void MainWindow::on_spinBox_FPS_page4_valueChanged(int value)
{
    ui->horizontalSlider_FPS_page4->setValue(value);
    scene3->setDesiredFPS(value);
}

void MainWindow::on_horizontalSlider_FPS_page4_valueChanged(int value)
{
    ui->spinBox_FPS_page4->setValue(value);
    scene3->setDesiredFPS(value);
}

void MainWindow::on_radioButton0_page5_clicked()
{
    scene4->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page5_clicked()
{
    scene4->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page5_clicked()
{
    scene4->setDisplayMode(2);
}
