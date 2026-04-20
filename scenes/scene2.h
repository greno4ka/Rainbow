#ifndef SCENE2_H
#define SCENE2_H

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene2 : public SceneBase
{
    /// DisplayMode values
    // 1 - only 1st rainbow beams
    // 2 - only 2nd rainbow beams
    int displayMode;
    int wavelength;

    double distance;

    Beams beams;

    void reinitializeBeams();
    void rayProcess(Beam beam);

public:
    Scene2();

    void setDisplayMode(int newDisplayMode);
    void setDistance(double newDistance);
    void setWavelength(int newWavelength);

    void display();
};

#endif // SCENE2_H
