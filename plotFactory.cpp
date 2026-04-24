#include "plotFactory.h"

static void attachCursor(QwtPlot *plot, bool darkThemeEnabled)
{
    QColor color = darkThemeEnabled ? Qt::white : Qt::black;

    QwtPlotPicker *picker = new QwtPlotPicker(
        QwtPlot::xBottom,
        QwtPlot::yLeft,
        plot->canvas()
    );

    picker->setStateMachine(new QwtPickerTrackerMachine());

    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerMode(QwtPicker::AlwaysOn);
    picker->setRubberBandPen(QPen(color, 1, Qt::DashLine));
    /// Font color
    picker->setTrackerPen(QPen(color));

    QFont font = picker->trackerFont();
    font.setBold(true);
    picker->setTrackerFont(font);
}

QwtPlot* createBasePlot(const QString &title,
                        const QString &xTitle,
                        const QString &yTitle,
                        bool darkThemeEnabled)
{
    QColor color = darkThemeEnabled
                       ? QColor(255, 255, 255, 70) // 70 because 100 is too bright
                       : QColor(0, 0, 0);

    QwtPlot *plot = new QwtPlot();

    plot->setTitle(title);
    plot->setAxisTitle(QwtPlot::xBottom, xTitle);
    plot->setAxisTitle(QwtPlot::yLeft, yTitle);
    plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(color, 1, Qt::DotLine));
    grid->attach(plot);

    return plot;
}

QwtPlot* createRefractiveIndexPlot(bool darkThemeEnabled)
{
    QColor color = darkThemeEnabled ? Qt::white : Qt::black;

    QwtPlot *plot = createBasePlot(
                        "Refractive index curve",
                        "Wavelength (nm)",
                        "Refractive index n",
                        darkThemeEnabled
                    );

    QVector<double> wavelength, index;
    for (int lambda = WAVE_MIN; lambda <= WAVE_MAX; lambda++) {
        wavelength.append(lambda);
        index.append(Beam(0, 0, 0, lambda, 0).refractIn());
    }

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setSamples(wavelength, index);
    curve->setPen(color, 2);
    curve->attach(plot);

    plot->setAxisAutoScale(QwtPlot::xBottom, true);
    plot->setAxisAutoScale(QwtPlot::yLeft, true);

    attachCursor(plot, darkThemeEnabled);

    return plot;
}

QwtPlot* createRainbowPlot(
    int rainbowMode,
    const QString& title,
    const QVector<double>& wavelengths,
    const std::function<double(double, double)>& phiFunc,
    bool darkThemeEnabled
    )
{
    QwtPlot *plot = createBasePlot(
        title,
        "y = h/r",
        "φ (degrees)",
        darkThemeEnabled
        );

    double yMin = (rainbowMode - 1) ? 0.8 : 0.7;
    double yMax = (rainbowMode - 1) ? 0.99 : 0.99;

    for (int i = 0; i < wavelengths.size(); i++) {
        QVector<double> xData, yData;
        double phiExt = (rainbowMode - 1) ? 1e9 : -1e9;
        double yExt = 0;
        double n = Beam(0, 0, 0, wavelengths[i], 0).refractIn();

        for (int lambda = WAVE_MIN; lambda <= WAVE_MAX; lambda++) {
            // y = h / r - normalized parameter
            double y = yMin + (yMax - yMin) * (lambda - WAVE_MIN) / (WAVE_MAX - WAVE_MIN);

            double phi = phiFunc(y, n) * 180.0 / M_PI;
            if (rainbowMode == 2) phi = 360 - phi;

            xData.push_back(y);
            yData.push_back(phi);

            if (rainbowMode == 1) {
                if (phi > phiExt) {
                    phiExt = phi;
                    yExt = y;
                }
            } else {
                if (phi < phiExt) {
                    phiExt = phi;
                    yExt = y;
                }
            }
        }

        int r, g, b;
        wavelengthToRGB(wavelengths[i], &r, &g, &b);

        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->setPen(QPen(QColor(r,g,b), 3));
        curve->setSamples(xData, yData);
        curve->attach(plot);

        QVector<double> xLine{yMin, yExt};
        QVector<double> yLine{phiExt, phiExt};

        QwtPlotCurve *line = new QwtPlotCurve();
        line->setPen(QPen(QColor(r,g,b), 2));
        line->setSamples(xLine, yLine);
        line->attach(plot);
    }

    double phiMin = (rainbowMode - 1) ? 50 : 39;
    double phiMax = (rainbowMode - 1) ? 60 : 42.5;
    plot->setAxisScale(QwtPlot::yLeft, phiMin, phiMax);
    /// x Axis is autoscaled!!! because I want yMax = 1 without extra pain
    plot->setAxisAutoScale(QwtPlot::xBottom, true);

    attachCursor(plot, darkThemeEnabled);

    return plot;
}