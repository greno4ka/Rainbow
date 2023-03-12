#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentStackWidgetPage = 0;
    scene0 = new Scene0();
    scene1 = new Scene1();
    scene2 = new Scene2();
    ui->glWidget->connectWithScene0(*scene0);
    ui->glWidget->connectWithScene1(*scene1);
    ui->glWidget->connectWithScene2(*scene2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_next_clicked()
{
    if (++currentStackWidgetPage == ui->stackedWidget->count())
        currentStackWidgetPage = 0;
    ui->stackedWidget->setCurrentIndex(currentStackWidgetPage);
}

void MainWindow::on_pushButton_prev_clicked()
{
    if (--currentStackWidgetPage < 0)
        currentStackWidgetPage = ui->stackedWidget->count() - 1;
    ui->stackedWidget->setCurrentIndex(currentStackWidgetPage);
}

void MainWindow::on_pushButton_fullscreen_clicked()
{
    isFullScreen() ? showNormal() : showFullScreen();
}

void MainWindow::on_pushButton_close_clicked()
{
    close();
}

void MainWindow::on_radioButton0_page3_clicked()
{

}

void MainWindow::on_radioButton1_page3_clicked()
{

}

void MainWindow::on_radioButton2_page3_clicked()
{

}

void MainWindow::on_pushButton_startpause_page3_clicked()
{

}

void MainWindow::on_doubleSpinBox_dist_page0_valueChanged(double value)
{
    scene0->setDistance(value);
    ui->horizontalSlider_dist_page0->setValue(value*100);
}

void MainWindow::on_horizontalSlider_dist_page0_valueChanged(int value)
{
    scene0->setDistance((double)value/100);
    ui->doubleSpinBox_dist_page0->setValue((double)value/100);
}

void MainWindow::on_spinBox_wave_page0_valueChanged(int value)
{
    scene0->setWavelength(value);
    ui->horizontalSlider_wave_page0->setValue(value);
}

void MainWindow::on_horizontalSlider_wave_page0_valueChanged(int value)
{
    scene0->setWavelength(value);
    ui->spinBox_wave_page0->setValue(value);
}

void MainWindow::on_radioButton0_page0_clicked()
{
    scene0->setDisplayMode(0);
}

void MainWindow::on_radioButton1_page0_clicked()
{
    scene0->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page0_clicked()
{
    scene0->setDisplayMode(2);
}

void MainWindow::on_pushButton_addBeam_page0_clicked()
{
    double d = ui->doubleSpinBox_dist_page0->value();
    int wl = ui->spinBox_wave_page0->value();
    scene0->addBeam(d, wl);
}

void MainWindow::on_pushButton_beamPrev_page0_clicked()
{
    if (!scene0->Beams.empty())
        scene0->decBeamStep();
}

void MainWindow::on_pushButton_beamNext_page0_clicked()
{
    if (!scene0->Beams.empty())
        scene0->incBeamStep();
}

void MainWindow::on_pushButton_clearScr_page0_clicked()
{
    scene0->Beams.clear();
    scene0->resetBeamStep();
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


void MainWindow::on_radioButton1_page1_clicked()
{
    scene1->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page1_clicked()
{
    scene1->setDisplayMode(2);
}

void MainWindow::on_pushButton_clearScr_page1_clicked()
{
    double d=0.86; // good inception with parallel beams
    int wl=600;
    ui->doubleSpinBox_dist_page1->setValue(d);
    ui->horizontalSlider_dist_page1->setValue(d*100);
    ui->spinBox_wave_page1->setValue(wl);
    ui->horizontalSlider_wave_page1->setValue(wl);
    ui->radioButton1_page1->setChecked(1);
    scene1->setDisplayMode(1);
    scene1->reinitializeBeams();
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

void MainWindow::on_spinBox_number_of_beams_page2_valueChanged(int value)
{
    scene2->setNumberOfBeams(ui->spinBox_number_of_beams_page2->value());
    ui->horizontalSlider_number_of_beams_page2->setValue(value);
}

void MainWindow::on_horizontalSlider_number_of_beams_page2_valueChanged(int value)
{
    scene2->setNumberOfBeams(ui->spinBox_number_of_beams_page2->value());
    ui->spinBox_number_of_beams_page2->setValue(value);
}

void MainWindow::on_radioButton1_page2_clicked()
{
    scene2->setDisplayMode(1);
}

void MainWindow::on_radioButton2_page2_clicked()
{
    scene2->setDisplayMode(2);
}

void MainWindow::on_checkBox_show_angle_page2_stateChanged(int value)
{
    scene2->setShowAngle(value);
}

void MainWindow::on_checkBox_show_rainbow_beams_page2_stateChanged(int value)
{
    scene2->setShowRainbow(value);
}

void MainWindow::on_pushButton_reset_settings_page2_clicked()
{

}

