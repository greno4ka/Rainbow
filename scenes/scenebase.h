#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <QOpenGLFunctions>
#include <QObject>

#include <cmath>
#include "config.h"
#include "beam.h"

class SceneBase : public QObject
{
    Q_OBJECT

signals:
    void requestUpdate();

protected:
    static const int ImageQuality = 100; // quality of drawing circles (drops, clouds...)
    static const int DropRadius = 5;

    int X, Y; // Window height and width
    double dpiScale;

    bool darkThemeEnabled;

    double offsetXFactor,
           offsetYFactor;

    double scaleXFactor,
           scaleYFactor;
    double sceneScale; // this is common zoom factor for X and Y scaling

    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0)
    {
        return X - (X/offsetXFactor) + x0 * std::min(X/scaleXFactor, Y/scaleYFactor) / sceneScale;
    }

    inline double y(double y0)
    {
        return Y - (Y/offsetYFactor) + y0 * std::min(X/scaleXFactor, Y/scaleYFactor) / sceneScale;
    }

    inline double r(double r0)
    {
        return r0 * std::min(X/scaleXFactor, Y/scaleYFactor) / sceneScale;
    }

    void drawRay(double x0, double y0, double x1, double y1);
    void drawInitialRay(double x1, double y1);
    void drawAngleArc(Beam &beam1, Beam &beam2, double x0, double y0, double radius, bool arcOnTop);

    void drawDrop();
    void drawAxes();

public:
    SceneBase(QObject *parent = nullptr);

    void updateXY(int newX, int newY);
    void setTheme(bool newDarkThemeEnabled);
    void setDPIScale(double newDPIScale);

    virtual void display() = 0;
};

#endif // SCENEBASE_H
