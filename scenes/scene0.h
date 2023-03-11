#ifndef SCENE0_H
#define SCENE0_H

#include <QOpenGLFunctions>

#include "beam.h"

class Scene0
{
    double distance;
    int wavelength;

public:
    Scene0();

    int getWavelength() const;
    void setWavelength(int newWavelength);
    double getDistance() const;
    void setDistance(double newDistance);

    const int Scale=10;
    int X,Y, // Window height and width
    Z,       // Coordinates center
    R;       // Radius of drop

    int N,
    radio_ch;

    Lst Beams;

    void draw_drop();
    void draw_axes();

    void display();


    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);

};

#endif // SCENE0_H
