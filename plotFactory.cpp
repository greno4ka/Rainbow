#include "plotFactory.h"

QwtPlot* createBasePlot(const QString &title,
                        const QString &xTitle,
                        const QString &yTitle)
{
    QwtPlot *plot = new QwtPlot();

    plot->setTitle(title);
    plot->setCanvasBackground(QColor(43, 43, 43));

    plot->setAxisTitle(QwtPlot::xBottom, xTitle);
    plot->setAxisTitle(QwtPlot::yLeft, yTitle);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(QColor(255,255,255,60), 0.5, Qt::DotLine));
    grid->attach(plot);

    plot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    return plot;
}

QwtPlot* createRefractiveIndexPlot()
{
    QwtPlot *plot = createBasePlot(
        "Refractive index curve",
        "Wavelength (nm)",
        "Refractive index n"
        );

    QVector<double> wl, n;
    Beam beam(1.0);

    int points = 600;

    for (int i = 0; i < points; i++)
    {
        double x = 380 + (780 - 380) * i / (points - 1);

        beam = Beam(0,0,0,x,1.0);

        wl.append(x);
        n.append(beam.refractIn());
    }

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setSamples(wl, n);
    curve->setPen(Qt::white, 2);
    curve->attach(plot);

    plot->setAxisScale(QwtPlot::xBottom, 380, 780);
    plot->setAxisScale(QwtPlot::yLeft, 1.325, 1.346);

    return plot;
}

QwtPlot* createPrimaryRainbowPlot()
{
    QwtPlot *plot = createBasePlot(
        "Primary rainbow",
        "y = h/r",
        "φ (degrees)"
        );

    double wavelengths[] = {410,450,480,520,570,600,650};
    int points = 600;

    for (int i = 0; i < 7; i++)
    {
        Beam tmp;
        tmp.setWavelength(wavelengths[i]);
        double n = tmp.refractIn();

        QVector<double> xData, yData;
        double maxPhi = -1e9;

        for (int j = 0; j < points; j++)
        {
            double y = 0.7 + (0.99 - 0.7) * j / (points - 1);
            double val = y / n;

            if (fabs(val) >= 1.0) continue;

            double phi = 4*asin(val) - 2*asin(y);
            double deg = phi * 180.0 / M_PI;

            xData << y;
            yData << deg;

            if (deg > maxPhi) maxPhi = deg;
        }

        int r,g,b;
        wavelengthToRGB(wavelengths[i],&r,&g,&b);

        QwtPlotCurve *curve = new QwtPlotCurve(QString("λ=%1").arg(wavelengths[i]));
        curve->setPen(QPen(QColor(r,g,b),2.5));
        curve->setSamples(xData,yData);
        curve->setZ(0);
        curve->attach(plot);

        // горизонталь
        QVector<double> xs{0.7, 0.86};
        QVector<double> ys{maxPhi, maxPhi};

        QwtPlotCurve *line = new QwtPlotCurve();
        line->setPen(QPen(QColor(r,g,b),1.5));
        line->setSamples(xs,ys);
        line->setZ(1);
        line->attach(plot);
    }

    plot->setAxisScale(QwtPlot::xBottom,0.7,0.99);
    plot->setAxisScale(QwtPlot::yLeft,39,42.5);

    return plot;
}

QwtPlot* createSecondaryRainbowPlot()
{
    QwtPlot *plot = createBasePlot(
        "Secondary rainbow",
        "y = h/r",
        "φ (degrees)"
        );

    double wavelengths[] = {410,450,480,520,570,600,650};
    int points = 600;

    for (int i = 0; i < 7; i++)
    {
        Beam tmp;
        tmp.setWavelength(wavelengths[i]);
        double n = tmp.refractIn();

        QVector<double> xData, yData;
        double minPhi = 1e9;

        for (int j = 0; j < points; j++)
        {
            double y = 0.8 + (0.99 - 0.8) * j / (points - 1);
            double val = y / n;

            if (fabs(val) >= 1.0) continue;

            double phi = M_PI + 6*asin(val) - 2*asin(y);
            double deg = 360.0 - phi * 180.0 / M_PI;

            xData << y;
            yData << deg;

            if (deg < minPhi) minPhi = deg;
        }

        int r,g,b;
        wavelengthToRGB(wavelengths[i],&r,&g,&b);

        QwtPlotCurve *curve = new QwtPlotCurve(QString("λ=%1").arg(wavelengths[i]));
        curve->setPen(QPen(QColor(r,g,b),2.5));
        curve->setSamples(xData,yData);
        curve->setZ(0);
        curve->attach(plot);

        QVector<double> xs{0.8, 0.95};
        QVector<double> ys{minPhi, minPhi};

        QwtPlotCurve *line = new QwtPlotCurve();
        line->setPen(QPen(QColor(r,g,b),1.5));
        line->setSamples(xs,ys);
        line->setZ(1);
        line->attach(plot);
    }

    plot->setAxisScale(QwtPlot::xBottom,0.8,1.0);
    plot->setAxisScale(QwtPlot::yLeft,50,60);

    return plot;
}