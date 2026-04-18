#ifndef PLOTFACTORY_H
#define PLOTFACTORY_H

#include <QPen>
#include <QColor>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>

#include "beam.h"
#include "wavelength.h"

QwtPlot* createBasePlot(const QString &title,
                        const QString &xTitle,
                        const QString &yTitle);
QwtPlot* createRefractiveIndexPlot();
QwtPlot* createPrimaryRainbowPlot();
QwtPlot* createSecondaryRainbowPlot();

#endif // PLOTFACTORY_H
