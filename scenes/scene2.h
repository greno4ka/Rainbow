#ifndef SCENE2_H
#define SCENE2_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene2 : public SceneBase
{
    int displayMode;
    int wavelength;
    int numberOfBeams;
    int showRainbow;
    int showAngle;

    double coordX, coordY, // point of crossing refracted beam with horizontal
           currentAngle;

    Beams beams;

    void reinitializeBeams();
    void draw_beam(Beam beam);

public:
    Scene2();

    void setDisplayMode(int newDisplayMode);
    void setWavelength(int newWavelength);
    void setNumberOfBeams(int newNumberOfBeams);
    void setShowRainbow(int newShowRainbow);
    void setShowAngle(int newShowAngle);

    void display();
    double getCoordX();
    double getCoordY();
    double getCurrentAngle() const;
    int getShowAngle() const;
};

#endif // SCENE2_H
