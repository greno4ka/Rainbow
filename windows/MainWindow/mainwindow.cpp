#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

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

    // ui->label_for_image->setAlignment(Qt::AlignCenter);
    // ui->label_for_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // ui->label_for_image->setMinimumSize(1, 1);

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
    changeTheme(darkThemeEnabled);

    // Apply fullscreen if needed
    if (fullscreenEnabled) {
        showFullScreen();
    }
    switchScene();
    currentSlide = "rainbow";

    QwtPlot *plot = new QwtPlot(ui->widget);
    plot->setTitle("Refractive index curve");
    plot->setAxisTitle(QwtPlot::xBottom, "Wavelength (nm)");
    plot->setAxisTitle(QwtPlot::yLeft, "Refractive index n");

    QVector<double> wl, n;

    Beam beam(1.0);

    int points = 600;
    double start = 400;
    double end = 650;

    for (int i = 0; i < points; i++)
    {
        double x = start + (end - start) * i / (points - 1);

        beam = Beam(0,0,0, x, 1.0);

        wl.append(x);
        n.append(beam.refractIn());
    }

    QwtPlotCurve *curve = new QwtPlotCurve("Polynomial model");
    curve->setSamples(wl, n);
    curve->setPen(Qt::white, 2, Qt::SolidLine);
    curve->setStyle(QwtPlotCurve::Lines);
    curve->setRenderHint (QwtPlotItem::RenderAntialiased, true);
    curve->attach(plot);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::white, 0.5, Qt::DotLine));
    grid->attach(plot);

    plot->setAxisScale(QwtPlot::xBottom, 400, 650);
    plot->resize(700, 400);
    plot->show();

    plot->replot();

    ui->widget->setLayout(new QVBoxLayout());
    ui->widget->layout()->addWidget(plot);


    QTimer::singleShot(0, this, &MainWindow::updateSlide);
}

QString MainWindow::getSlidePath(QString slideName)
{
    QString theme = darkThemeEnabled ? "white" : "black";
    return QString(":/%1_%2_%3.png")
        .arg(slideName)
        .arg(theme)
        .arg(appLanguage);
}

void MainWindow::updateSlide()
{
    if (currentSlide == "rainbow")
        slidePixmap = QPixmap(":/double-rainbow-1000.jpg");
    else {
        slidePixmap = QPixmap(getSlidePath(currentSlide));
    }
    // ui->label_for_image->setPixmap(
    //     slidePixmap.scaled(
    //         ui->label_for_image->size(),
    //         Qt::KeepAspectRatio,
    //         Qt::SmoothTransformation
    //         )
    //     );
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    updateSlide();
}

void MainWindow::changeLanguage(const QString &language)
{
    appLanguage = language;
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

    delete settingsWindow;

    delete settings;
    delete ui;
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

    ui->menuWidget->setCurrentIndex(currentStackWidgetPage);
    glWidget->setSceneNumber(currentStackWidgetPage);
    switchWidget();
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
    updateSlide();
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