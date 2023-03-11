#ifndef SCENE1_H
#define SCENE1_H

#include "beam.h"

class Scene1
{
    double distance;
    int wavelength;
    int displayMode;

    static const int SceneScale = 10;
    int X,Y; // Window height and width
public:
    Scene1();

    static const int DropRadius = 5;

    void updateXY(int newX, int newY);

    void setWavelength(int newWavelength);
    void setDistance(double newDistance);
    void setDisplayMode(int newDisplayMode);

    Lst Beams;

    void draw_drop();
    void draw_axes();

    void display();


    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);
};

#endif // SCENE1_H
