#ifndef SCENE6_H
#define SCENE6_H

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene6 : public SceneBase
{
    Beam beam;
    Beam normal; // x=0
    Beam refracted;

    void drawWater();
    void drawAxes();
    void rayProcess();
    void drawAngleArc(Beam &beam, double radius, bool arcOnTop);

public:
    Scene6();

    double getCoordX(double x0);
    double getCoordY(double y0);

    void display();
};

#endif // SCENE6_H
