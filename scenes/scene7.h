#ifndef SCENE7_H
#define SCENE7_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene7 : public SceneBase
{
    int displayMode;

    Beam beam;
    Beam normal;

    void rayProcess();
    void drawAngleArc(Beam &beam, double radius, bool arcOnTop);

public:
    Scene7();

    void setDisplayMode(int newDisplayMode);

    double getCoordX(double x0);
    double getCoordY(double y0);

    void display();
};

#endif // SCENE7_H
