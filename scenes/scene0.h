#ifndef SCENE0_H
#define SCENE0_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene0 : public SceneBase
{
    static const int MaxNumberOfReflections = 5;

    int displayMode;
    int wavelength;
    int beamStep;

    double distance;

    Beams beams;

    void draw_beam(Beam beam);

public:
    Scene0();

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

#endif // SCENE0_H
