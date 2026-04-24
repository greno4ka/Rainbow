#ifndef PLOTFACTORY_H
#define PLOTFACTORY_H

#include <QPen>
#include <QColor>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

#include "config.h"
#include "beam.h"
#include "wavelength.h"

QwtPlot* createBasePlot(const QString &title,
                        const QString &xTitle,
                        const QString &yTitle,
                        bool darkThemeEnabled);

QwtPlot* createRefractiveIndexPlot(bool darkThemeEnabled);

QwtPlot* createRainbowPlot(
    int rainbowMode,
    const QString& title,
    const QVector<double>& wavelengths,
    const std::function<double(double, double)>& phiFunc,
    bool darkThemeEnabled
    );

#endif // PLOTFACTORY_H
