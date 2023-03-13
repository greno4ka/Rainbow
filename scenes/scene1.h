#ifndef SCENE1_H
#define SCENE1_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene1 : public SceneBase
{
    int displayMode;
    int wavelength;

    double distance;

    Beams beams;

    void reinitializeBeams();
    void draw_beam(Beam beam);

public:
    Scene1();

    void setDisplayMode(int newDisplayMode);
    void setWavelength(int newWavelength);
    void setDistance(double newDistance);

    void display();
};

#endif // SCENE1_H
