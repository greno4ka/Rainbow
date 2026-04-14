#include "mainwindow.h"
#include "ui_mainwindow.h"

static QString loadStyle(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
        return {};

    return QString::fromUtf8(file.readAll());
}

MainWindow::MainWindow(int programMode, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create settings window first
    settingsWindow = new SettingsWindow(this);
    connect(settingsWindow, &SettingsWindow::language_change,
            this, &MainWindow::retranslate);
    connect(settingsWindow, &SettingsWindow::theme_change,
            this, &MainWindow::onThemeChanged);
    connect(settingsWindow, &SettingsWindow::multisampling_change,
            this, &MainWindow::onMultisamplingChanged);
    connect(settingsWindow, &SettingsWindow::fullscreen_change,
            this, &MainWindow::onFullscreenChanged);

    // Load initial settings before creating GLWidget
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString settingsFilePath = configPath + "/settings.ini";
    QSettings settings(settingsFilePath, QSettings::IniFormat);

    // Apply initial settings
    bool multisamplingEnabled = settings.value("multisampling", true).toBool();
    bool fullscreenEnabled = settings.value("fullscreen", false).toBool();
    bool isDarkTheme = settings.value("theme", 0).toInt() == 0;

    // Load language setting
    translator = new QTranslator(this);
    int languageIndex = settings.value("language", 0).toInt();
    QString langCode;
    switch (languageIndex) {
        case 1: langCode = "ru"; break;
        case 2: langCode = "fr"; break;
        default: langCode = "en"; break;
    }
    if (languageIndex > 0) {  // Only load if not English (default)
        if (translator->load(QString("rainbow_") + langCode)) {
            qApp->installTranslator(translator);
            ui->retranslateUi(this);
        }
    }
    settingsWindow->setTranslator(translator);

    //rainbowPixmap = QPixmap(":/double-rainbow-1000.jpg");
    ui->rainbow->setAlignment(Qt::AlignCenter);
    ui->rainbow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //ui->rainbow->setPixmap(rainbowPixmap);
    ui->rainbow->setMinimumSize(1, 1);

    glWidget = new GLWidget(this);
    glWidget3d = new GLWidget3D(this);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    glWidget->setSizePolicy(sizePolicy);
    glWidget3d->setSizePolicy(sizePolicy);

    // Apply multisampling setting
    QSurfaceFormat format;
    format.setSamples(multisamplingEnabled ? 8 : 0);
    glWidget->setFormat(format);
    glWidget3d->setFormat(format);

    ui->glWidgetLayout->addWidget(glWidget);
    ui->glWidgetLayout->addWidget(glWidget3d);
    currentStackWidgetPage = programMode;

    // Create scenes
    scene1 = new Scene1();
    scene2 = new Scene2();
    scene3 = new Scene3();
    scene4 = new Scene4();
    scene5 = new Scene5();

    scenex = new SceneX();

    glWidget->connectWithScene1(*scene1);
    glWidget->connectWithScene2(*scene2);
    glWidget->connectWithScene3(*scene3);
    glWidget->connectWithScene4(*scene4);
    glWidget->connectWithScene5(*scene5);

    glWidget3d->connectWithSceneX(*scenex);

    // Apply theme
    applyTheme(isDarkTheme);

    // Apply fullscreen if needed
    if (fullscreenEnabled) {
        showFullScreen();
    }
    switchScene();
    QTimer::singleShot(0, this, &MainWindow::updateSlide);
}

QString MainWindow::getSlidePath()
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString settingsFilePath = configPath + "/settings.ini";
    QSettings settings(settingsFilePath, QSettings::IniFormat);
    bool isDarkTheme = settings.value("theme", 0).toInt() == 0;
    int languageIndex = settings.value("language", 0).toInt();
    QString langCode;
    switch (languageIndex) {
    case 1: langCode = "ru"; break;
    case 2: langCode = "fr"; break;
    default: langCode = "en"; break;
    }
    QString theme = isDarkTheme ? "white" : "black";
    return QString(":/slide2_%1_%2.png")
        .arg(theme)
        .arg(langCode);
}

void MainWindow::updateSlide()
{
    slidePixmap = QPixmap(getSlidePath());

    ui->rainbow->setPixmap(
        slidePixmap.scaled(
            ui->rainbow->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    updateSlide();
}

void MainWindow::retranslate()
{
    ui->retranslateUi(this);
    updateSlide();
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
        glWidget3d->hide();
        ui->presentationWidget->show();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        glWidget->show();
        glWidget3d->hide();
        ui->presentationWidget->hide();
        break;
    case 6:
        glWidget->hide();
        glWidget3d->show();
        ui->presentationWidget->hide();
        break;
    }
    updateSlide();
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
    case 6:
        ui->label_pageName->setText(tr("Formation of a rainbow"));
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
    QString style = loadStyle(isDark ? ":/dark.qss"
                                     : ":/light.qss");

    // Apply the stylesheet to both windows
    this->setStyleSheet(style);
    settingsWindow->setStyleSheet(style);

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
    updateSlide();
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

void MainWindow::on_spinBox_number_of_beams_page5_valueChanged(int value)
{
    scene5->setNumberOfBeams(value);
    ui->horizontalSlider_number_of_beams_page5->setValue(value);
}


void MainWindow::on_horizontalSlider_number_of_beams_page5_valueChanged(int value)
{
    scene5->setNumberOfBeams(value);
    ui->spinBox_number_of_beams_page5->setValue(value);
}


void MainWindow::on_spinBox_dispersion_quality_page5_valueChanged(int value)
{
    scene5->setBeamQuality(value);
    ui->horizontalSlider_dispersion_quality_page5->setValue(value);
}


void MainWindow::on_horizontalSlider_dispersion_quality_page5_valueChanged(int value)
{
    scene5->setBeamQuality(value);
    ui->spinBox_dispersion_quality_page5->setValue(value);
}


void MainWindow::on_pushButton_reset_settings_page5_clicked()
{
    scene5->setDisplayMode(0);
    scene5->setBeamQuality(10);
    scene5->setNumberOfBeams(30);
    ui->spinBox_dispersion_quality_page5->setValue(10);
    ui->horizontalSlider_dispersion_quality_page5->setValue(10);
    ui->spinBox_number_of_beams_page5->setValue(30);
    ui->horizontalSlider_number_of_beams_page5->setValue(30);
    ui->radioButton0_page5->setChecked(1);

}

