#ifndef SCENE7_H
#define SCENE7_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene7 : public SceneBase
{
    Beam beam;
    Beam normal; // x=0
    Beam refracted;

    void drawWater();
    void drawAxes();
    void rayProcess();
    void drawAngleArc(Beam &beam, double radius, bool arcOnTop);

public:
    Scene7();

    double getCoordX(double x0);
    double getCoordY(double y0);

    void display();
};

#endif // SCENE7_H
