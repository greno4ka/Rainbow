#ifndef SCENE0_H
#define SCENE0_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene0 : public SceneBase
{
    double distance;
    int displayMode;
    int wavelength;
    int beamStep;

    static const int MaxNumberOfReflections = 5;

public:
    Scene0();

    void setWavelength(int newWavelength);
    void setDistance(double newDistance);
    void setDisplayMode(int newDisplayMode);

    Lst Beams;

    void addBeam(double newDistance, int newWavelength);

    void draw_beam(Beam beam);

    void display();

    void incBeamStep();
    void decBeamStep();
    void resetBeamStep();
};

#endif // SCENE0_H
