#ifndef SCENE2_H
#define SCENE2_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene2 : public SceneBase
{
    int displayMode;
    int wavelength;

    double distance;

    Beams beams;

    void reinitializeBeams();
    void draw_beam(Beam beam);

public:
    Scene2();

    void setDisplayMode(int newDisplayMode);
    void setWavelength(int newWavelength);
    void setDistance(double newDistance);

    void display();
};

#endif // SCENE2_H
