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