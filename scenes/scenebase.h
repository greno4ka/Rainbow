#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <QOpenGLFunctions>

#include "beam.h"

class SceneBase
{
    static const int SceneScale = 10;

protected:
    static const int ImageQuality = 100;
    static const int DropRadius = 5;

    int X,Y; // Window height and width

    /// Methods of recalculation decart coords to screen coords
    double x(double x0);
    double y(double y0);
    double r(double r0);

    void drawLine(double x0, double y0, double x1, double y1);

    void draw_drop();
    void draw_axes();

public:
    SceneBase();
    void draw1stRainbow(Beam beam);
    void draw2ndRainbow(Beam beam);

    void updateXY(int newX, int newY);
    virtual void display() = 0;
};

#endif // SCENEBASE_H
