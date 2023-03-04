#ifndef SCENE0_H
#define SCENE0_H

#include <QOpenGLFunctions>
#include <QList>

#include "line.h"

class Scene0
{
public:
    Scene0();

    const int Scale=10;
    int X,Y, // Window height and width
    Z,       // Coordinates center
    R;       // Radius of drop

    int N,
    radio_ch;

    typedef QList<Line> Lst;
    Lst Beams;
    double d,wl;

    void draw_drop();


    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);
};

#endif // SCENE0_H
