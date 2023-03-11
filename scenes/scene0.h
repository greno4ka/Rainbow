#ifndef SCENE0_H
#define SCENE0_H

#include <QOpenGLFunctions>

#include "beam.h"

class Scene0
{
    double distance;
    int wavelength;
    int displayMode;
    int beamStep;

    static const int MaxNumberOfReflections = 5;

public:
    Scene0();

    static const int DropRadius = 5;

    void setWavelength(int newWavelength);
    void setDistance(double newDistance);
    void setDisplayMode(int newDisplayMode);

    const int Scale=10;
    int X=1366,Y=768; // Window height and width

    Lst Beams;

    void draw_drop();
    void draw_axes();

    void display();


    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);

    void incBeamStep();
    void decBeamStep();
    void resetBeamStep();
};

#endif // SCENE0_H
