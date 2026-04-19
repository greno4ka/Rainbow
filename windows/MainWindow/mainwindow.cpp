#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plotFactory.h"

static QString loadStyle(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
        return {};

    return QString::fromUtf8(file.readAll());
}

MainWindow::MainWindow(int programMode, QTranslator *newTranslator, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUIDefaults();

    // Load initial settings before creating GLWidget
    configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settingsFilePath = configPath + "/settings.ini";
    settings = new QSettings(settingsFilePath, QSettings::IniFormat);

    // Read initial settings
    multisamplingEnabled = settings->value("multisampling", false).toBool();
    fullscreenEnabled = settings->value("fullscreen", false).toBool();
    darkThemeEnabled = settings->value("theme", 0).toInt() == 0;

    translator = newTranslator;

    // Load language setting
    appLanguage = settings->value("language", "en").toString();

    if (!translator->load("rainbow_" + appLanguage)) {
        QMessageBox::critical(this, tr("Error: 0xDEADBEE"), tr("Can't change language."));
    }

    // Install language setting
    qApp->removeTranslator(translator);
    qApp->installTranslator(translator);

    // Finally use language setting to rewrite all text
    ui->retranslateUi(this);

    // Create all windows after installing translator
    // It should use installed language
    settingsWindow = new SettingsWindow(translator,this);
    connect(settingsWindow, &SettingsWindow::language_change,
            this, &MainWindow::changeLanguage);
    connect(settingsWindow, &SettingsWindow::theme_change,
            this, &MainWindow::changeTheme);
    connect(settingsWindow, &SettingsWindow::multisampling_change,
            this, &MainWindow::changeMultisampling);
    connect(settingsWindow, &SettingsWindow::fullscreen_change,
            this, &MainWindow::changeFullscreen);

    ui->label_for_image->setAlignment(Qt::AlignCenter);
    ui->label_for_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->label_for_image->setMinimumSize(1, 1);

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

    ui->glWidgetLayout->addWidget(glWidget3d);
    currentMenuWidgetPage = programMode;
    currentSlideWidgetPage = 0;

    ui->glWidgetLayout_slide1->addWidget(glWidget);

    // Create scenes
    scene1 = new Scene1();
    scene2 = new Scene2();
    scene3 = new Scene3();
    scene4 = new Scene4();
    scene5 = new Scene5();

    scenex = new SceneX();

    scene6 = new Scene6();
    scene7 = new Scene7();

    glWidget->connectWithScene1(*scene1);
    glWidget->connectWithScene2(*scene2);
    glWidget->connectWithScene3(*scene3);
    glWidget->connectWithScene4(*scene4);
    glWidget->connectWithScene5(*scene5);

    glWidget->connectWithScene6(*scene6);
    glWidget->connectWithScene7(*scene7);

    glWidget3d->connectWithSceneX(*scenex);

    // Apply theme
    changeTheme(darkThemeEnabled);

    // Apply fullscreen if needed
    if (fullscreenEnabled) {
        showFullScreen();
    }
    switchPage();
    ui->slideWidget->setCurrentIndex(currentSlideWidgetPage);

    ui->qwtWidget_slide1->setLayout(new QVBoxLayout());
    ui->qwtWidget_slide1->layout()->addWidget(createRefractiveIndexPlot());

    ui->qwtWidget_slide2->setLayout(new QVBoxLayout());
    ui->qwtWidget_slide2->layout()->addWidget(createPrimaryRainbowPlot());

    ui->qwtWidget_slide3->setLayout(new QVBoxLayout());
    ui->qwtWidget_slide3->layout()->addWidget(createSecondaryRainbowPlot());

    QTimer::singleShot(0, this, &MainWindow::updateRainbowImage);
}

void MainWindow::initUIDefaults()
{
    ui->doubleSpinBox_dist_page1->setValue(SCENE1_DISTANCE_08);
    ui->horizontalSlider_dist_page1->setValue(SCENE1_DISTANCE_08*100);
    ui->spinBox_wave_page1->setValue(SCENE1_WAVELENGTH_600);
    ui->horizontalSlider_wave_page1->setValue(SCENE1_WAVELENGTH_600);

    ui->doubleSpinBox_dist_page2->setValue(SCENE2_DISTANCE_086);
    ui->horizontalSlider_dist_page2->setValue(SCENE2_DISTANCE_086*100);
    ui->spinBox_wave_page2->setValue(SCENE2_WAVELENGTH_600);
    ui->horizontalSlider_wave_page2->setValue(SCENE2_WAVELENGTH_600);

    ui->spinBox_number_of_beams_page3->setValue(SCENE3_NUMBER_OF_BEAMS_30);
    ui->horizontalSlider_number_of_beams_page3->setValue(SCENE3_NUMBER_OF_BEAMS_30);
    ui->spinBox_wave_page3->setValue(SCENE3_WAVELENGTH_600);
    ui->horizontalSlider_wave_page3->setValue(SCENE3_WAVELENGTH_600);

    ui->spinBox_number_of_beams_page5->setValue(SCENE5_NUMBER_OF_BEAMS);
    ui->horizontalSlider_number_of_beams_page5->setValue(SCENE5_NUMBER_OF_BEAMS);
    ui->spinBox_dispersion_quality_page5->setValue(SCENE5_BEAM_QUALITY_10);
    ui->horizontalSlider_dispersion_quality_page5->setValue(SCENE5_BEAM_QUALITY_10);
}

void MainWindow::updateRainbowImage()
{
    rainbowPixmap = QPixmap(":/double-rainbow-1000.jpg");
    ui->label_for_image->setPixmap(
        rainbowPixmap.scaled(
            ui->label_for_image->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    updateRainbowImage();
}

void MainWindow::changeLanguage(const QString &language)
{
    appLanguage = language;
    ui->retranslateUi(this);
}

MainWindow::~MainWindow()
{
    delete scene1;
    delete scene2;
    delete scene3;
    delete scene4;
    delete scene5;

    delete scene6;
    delete scene7;

    delete settingsWindow;

    delete settings;
    delete ui;
}

void MainWindow::switchPage()
{
    /// Change page name
    switch (currentMenuWidgetPage) {
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

    /// decide what to with widgets
    switch (currentMenuWidgetPage) {
    case 0:
        glWidget3d->hide();
        ui->slideWidget->show();
        switchSlide();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        glWidget->setSceneNumber(currentMenuWidgetPage);
        ui->glWidgetLayout->addWidget(glWidget);
        glWidget->show();
        glWidget3d->hide();
        ui->slideWidget->hide();
        break;
    case 6:
        glWidget->hide();
        glWidget3d->show();
        ui->slideWidget->hide();
        break;
    }

    ui->menuWidget->setCurrentIndex(currentMenuWidgetPage);
    if (currentSlideWidgetPage == 0) updateRainbowImage(); /// Should be called after changing window
}

void MainWindow::switchSlide()
{
    switch (currentSlideWidgetPage) {
    case 0:
        glWidget->hide();
        break;
    case 1:
        ui->glWidgetLayout_slide1->addWidget(glWidget);
        glWidget->setSceneNumber(6);
        glWidget->show();
        break;
    case 2:
        ui->glWidgetLayout_slide2->addWidget(glWidget);
        glWidget->setSceneNumber(7);
        scene7->setDisplayMode(0);
        glWidget->show();
        break;
    case 3:
        ui->glWidgetLayout_slide3->addWidget(glWidget);
        glWidget->setSceneNumber(7);
        scene7->setDisplayMode(1);
        glWidget->show();
        break;
    }

    ui->slideWidget->setCurrentIndex(currentSlideWidgetPage);
    if (currentSlideWidgetPage == 0) updateRainbowImage(); /// Should be called after changing window
}

void MainWindow::changeTheme(bool isDark)
{
    darkThemeEnabled = isDark;

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
}

void MainWindow::changeMultisampling(bool enabled)
{
    if (glWidget) {
        QSurfaceFormat format = glWidget->format();
        format.setSamples(enabled ? 8 : 0);
        glWidget->setFormat(format);
        glWidget->hide();  // Force a context recreation
        glWidget->show();
    }
}

void MainWindow::changeFullscreen(bool enabled)
{
    if (enabled) {
        showFullScreen();
    } else {
        showNormal();
    }
}