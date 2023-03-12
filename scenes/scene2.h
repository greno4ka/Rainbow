#ifndef SCENE2_H
#define SCENE2_H

#include <QOpenGLFunctions>

#include "beam.h"

class Scene2
{
    int wavelength;
    int numberOfBeams;
    int displayMode;

    static const int SceneScale = 10;
    int X,Y; // Window height and width
public:
    Scene2();

    static const int DropRadius = 5;

    Lst Beams;

    void reinitializeBeams();

    void updateXY(int newX, int newY);

    void setWavelength(int newWavelength);
    void setNumberOfBeams(int newNumberOfBeams);
    void setDisplayMode(int newDisplayMode);


    void draw_drop();
    void draw_axes();

    void draw_beam(Beam beam);
    void display();


    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);
};

#endif // SCENE2_H
