#ifndef SCENE1_H
#define SCENE1_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene1 : public SceneBase
{
    static const int MaxNumberOfReflections = 5;

    int displayMode;
    int wavelength;
    int beamStep;

    double distance;

    Beams beams;

    void draw_beam(Beam beam);

public:
    Scene1();

    void setDisplayMode(int newDisplayMode);
    void setWavelength(int newWavelength);
    void setDistance(double newDistance);
    void addBeam(double newDistance, int newWavelength);
    void clearBeams();
    bool ifNoBeams();
    void incBeamStep();
    void decBeamStep();
    void resetBeamStep();

    void display();
};

#endif // SCENE1_H
