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

    static const int SceneScale = 10;
    int X,Y; // Window height and width

public:
    Scene0();

    static const int DropRadius = 5;

    void updateXY(int newX, int newY);

    void setWavelength(int newWavelength);
    void setDistance(double newDistance);
    void setDisplayMode(int newDisplayMode);

    Lst Beams;

    void draw_drop();
    void draw_axes();

    void draw_beam(Beam beam);

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
