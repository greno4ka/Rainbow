#ifndef SCENE7_H
#define SCENE7_H

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene7 : public SceneBase
{
    int displayMode;

    Beam beam;
    Beam normal;

    void rayProcess();
    void drawRadiusDash(double x0, double y0);

public:
    Scene7();

    void setDisplayMode(int newDisplayMode);
    int getDisplaymode();

    double getCoordX(double x0);
    double getCoordY(double y0);

    void display();
};

#endif // SCENE7_H
