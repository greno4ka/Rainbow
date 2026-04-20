#ifndef SCENE3_H
#define SCENE3_H

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene3 : public SceneBase
{
    /// DisplayMode values
    // 1 - only 1st rainbow beams
    // 2 - only 2nd rainbow beams
    int displayMode;
    int wavelength;
    int numberOfBeams;

    int showRainbow;
    int showAngle;

    double coordX, coordY, // point of crossing refracted beam with horizontal
           bestAngle;      // if 0, dont show it

    Beams beams;

    void reinitializeBeams();
    void rayProcess(Beam beam);

public:
    Scene3();

    void resetScene();

    void setDisplayMode(int newDisplayMode);
    void setWavelength(int newWavelength);
    void setNumberOfBeams(int newNumberOfBeams);
    void setShowRainbow(int newShowRainbow);
    void setShowAngle(int newShowAngle);

    void display();
    double getCoordX();
    double getCoordY();
    double getBestAngle() const;
    int getShowAngle() const;
};

#endif // SCENE3_H
