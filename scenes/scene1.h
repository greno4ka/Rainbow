#ifndef SCENE1_H
#define SCENE1_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene1 : public SceneBase
{
    double distance;
    int wavelength;
    int displayMode;
public:
    Scene1();

    Lst Beams;

    void reinitializeBeams();

    void setWavelength(int newWavelength);
    void setDistance(double newDistance);
    void setDisplayMode(int newDisplayMode);


    void draw_beam(Beam beam);
    void display();

};

#endif // SCENE1_H
