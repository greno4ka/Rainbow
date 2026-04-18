#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>


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
    switchScene();
    ui->slideWidget->setCurrentIndex(currentSlideWidgetPage);

    QwtPlot *plot = new QwtPlot(ui->qwtWidget_slide1);
    plot->setTitle("Refractive index curve");
    plot->setAxisTitle(QwtPlot::xBottom, "Wavelength (nm)");
    plot->setAxisTitle(QwtPlot::yLeft, "Refractive index n");

    QVector<double> wl, n;

    Beam beam(1.0);

    int points = 600;
    double start = 380;
    double end = 780;

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

    plot->setAxisScale(QwtPlot::xBottom, 380, 780);
    plot->setAxisScale(QwtPlot::yLeft, 1.325, 1.346);
    /*
    plot->setAxisAutoScale(QwtPlot::yLeft, false);
    plot->setAxisMaxMajor(QwtPlot::yLeft, 10);
    plot->setAxisMaxMinor(QwtPlot::yLeft, 5);*/
    plot->show();

    plot->replot();


    QwtPlot *plot1 = new QwtPlot();

    plot1->setCanvasBackground(QColor(43, 43, 43));

    QwtPlotGrid *grid1 = new QwtPlotGrid();
    grid1->setPen(QPen(QColor(255, 255, 255, 60), 0.5, Qt::DotLine));
    grid1->attach(plot1);

    plot1->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    double wavelengths[] = {410, 450, 480, 520, 570, 600, 650};

    const int nCount = 7;

    for (int i = 0; i < nCount; i++)
    {
        Beam tmp = Beam();
        tmp.setWavelength(wavelengths[i]);
        double n = tmp.refractIn();

        QVector<double> xData;
        QVector<double> yData;

        double maxPhi = -1e9;

        for (int j = 0; j < points; j++)
        {
            double y = 0.7 + (0.99 - 0.7) * j / (points - 1);

            double val = y / n;

            if (val >= 1.0 || val <= -1.0)
                continue;

            double phi = 4.0 * std::asin(val) - 2.0 * std::asin(y);
            double phiDeg = phi * 180.0 / M_PI;

            xData.push_back(y);
            yData.push_back(phiDeg);

            if (phiDeg > maxPhi)
                maxPhi = phiDeg;
        }

        QwtPlotCurve *curve1 = new QwtPlotCurve();
        curve1->setTitle(QString("λ = %1").arg(wavelengths[i]));
        int r,g,b;
        wavelengthToRGB(wavelengths[i],&r,&g,&b);
        curve1->setPen(QPen(QColor(r, g, b), 2.5));
        curve1->setSamples(xData, yData);
        curve1->setZ(0.0);
        curve1->attach(plot1);

        QVector<double> xSeg;
        QVector<double> ySeg;

        double xEnd = 0.86;   // фиксированная граница

        xSeg << 0.7 << xEnd;   // или твой xmin (как в графике)
        ySeg << maxPhi << maxPhi;

        QwtPlotCurve *line = new QwtPlotCurve();
        line->setPen(QPen(QColor(r, g, b), 1.5));
        line->setSamples(xSeg, ySeg);
        line->setZ(1.0);
        line->attach(plot1);
    }

    plot1->setAxisTitle(QwtPlot::xBottom, "y = h/r");
    plot1->setAxisTitle(QwtPlot::yLeft, "φ (degrees)");

    plot1->setAxisScale(QwtPlot::xBottom, 0.7, 0.99);
    plot1->setAxisScale(QwtPlot::yLeft, 39, 42.5);



    QwtPlot *plot2 = new QwtPlot();

    plot2->setCanvasBackground(QColor(43, 43, 43));

    QwtPlotGrid *grid2 = new QwtPlotGrid();
    grid2->setPen(QPen(QColor(255, 255, 255, 60), 0.5, Qt::DotLine));
    grid2->attach(plot2);

    plot2->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    for (int i = 0; i < nCount; i++)
    {
        Beam tmp = Beam();
        tmp.setWavelength(wavelengths[i]);
        double n = tmp.refractIn();

        QVector<double> xData;
        QVector<double> yData;

        double minPhi = 1e9;

        for (int j = 0; j < points; j++)
        {
            double y = 0.8 + (0.99 - 0.8) * j / (points - 1);

            double val = y / n;

            // if (val >= 1.0 || val <= -1.0)
            //     continue;

            double phi = M_PI + 6.0 * std::asin(val) - 2.0 * std::asin(y);
            double phiDeg = 360 - phi * 180.0 / M_PI;

            xData.push_back(y);
            yData.push_back(phiDeg);

            if (phiDeg < minPhi)
                minPhi = phiDeg;
        }

        QwtPlotCurve *curve2 = new QwtPlotCurve();
        curve2->setTitle(QString("λ = %1").arg(wavelengths[i]));
        int r,g,b;
        wavelengthToRGB(wavelengths[i],&r,&g,&b);
        curve2->setPen(QPen(QColor(r, g, b), 2.5));
        curve2->setSamples(xData, yData);
        curve2->setZ(0.0);
        curve2->attach(plot2);

        QVector<double> xSeg;
        QVector<double> ySeg;

        double xEnd = 0.95;   // фиксированная граница

        xSeg << 0.8 << xEnd;   // или твой xmin (как в графике)
        ySeg << minPhi << minPhi;

        QwtPlotCurve *line = new QwtPlotCurve();
        line->setPen(QPen(QColor(r, g, b), 1.5));
        line->setSamples(xSeg, ySeg);
        line->setZ(1.0);
        line->attach(plot2);
    }

    plot2->setAxisTitle(QwtPlot::xBottom, "y = h/r");
    plot2->setAxisTitle(QwtPlot::yLeft, "φ (degrees)");

    plot2->setAxisScale(QwtPlot::xBottom, 0.8, 1);
    plot2->setAxisScale(QwtPlot::yLeft, 50, 60);


    plot->replot();
    plot1->replot();
    plot2->replot();


    ui->qwtWidget_slide1->setLayout(new QVBoxLayout());
    ui->qwtWidget_slide1->layout()->addWidget(plot);

    ui->qwtWidget_slide2->setLayout(new QVBoxLayout());
    ui->qwtWidget_slide2->layout()->addWidget(plot1);

    ui->qwtWidget_slide3->setLayout(new QVBoxLayout());
    ui->qwtWidget_slide3->layout()->addWidget(plot2);

    QTimer::singleShot(0, this, &MainWindow::updateSlide);
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
    if (currentSlideName == "rainbow")
        slidePixmap = QPixmap(":/double-rainbow-1000.jpg");
    else {
        slidePixmap = QPixmap(getSlidePath(currentSlideName));
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

    delete scene6;
    delete scene7;

    delete settingsWindow;

    delete settings;
    delete ui;
}

void MainWindow::switchWidget()
{
    switch (currentMenuWidgetPage) {
    case 0:
        switch(currentSlideWidgetPage) {
        case 1:
            ui->glWidgetLayout_slide1->addWidget(glWidget);
        glWidget->setSceneNumber(6);
        case 2:
            ui->glWidgetLayout_slide2->addWidget(glWidget);
            glWidget->setSceneNumber(7);
        case 3:
            ui->glWidgetLayout_slide3->addWidget(glWidget);
            glWidget->setSceneNumber(7);
        }

        glWidget->show();
        glWidget3d->hide();
        ui->slideWidget->show();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
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
    updateSlide();
}

void MainWindow::switchScene()
{
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

    ui->menuWidget->setCurrentIndex(currentMenuWidgetPage);
    glWidget->setSceneNumber(currentMenuWidgetPage);
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