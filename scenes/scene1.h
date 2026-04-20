#ifndef SCENE1_H
#define SCENE1_H

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene1 : public SceneBase
{
    static const int MaxNumberOfReflections = 5;

    /// DisplayMode values
    // 0 - show all beams
    // 1 - only 1st rainbow beams
    // 2 - only 2nd rainbow beams
    int displayMode;
    // current beam step from 0 to MaxNumberOfReflections
    int beamStep;

    Beams beams;

    void rayProcess(Beam beam);

public:
    Scene1();

    void setDisplayMode(int newDisplayMode);
    void addBeam(double newDistance, int newWavelength);
    void clearBeams();
    bool ifNoBeams();
    void incBeamStep();
    void decBeamStep();

    void display();
};

#endif // SCENE1_H
