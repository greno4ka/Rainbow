#ifndef SCENE2_H
#define SCENE2_H

#include <QOpenGLFunctions>
#include <QPainter>

#include "scenebase.h"
#include "beam.h"

class Scene2 : public SceneBase
{
    int wavelength;
    int numberOfBeams;
    int displayMode;

    int showRainbow;
    int showAngle;

public:
    Scene2();

    Lst Beams;

    void reinitializeBeams();

    void setWavelength(int newWavelength);
    void setNumberOfBeams(int newNumberOfBeams);
    void setDisplayMode(int newDisplayMode);

    void draw_beam(Beam beam);
    void display();

    void setShowRainbow(int newShowRainbow);
    void setShowAngle(int newShowAngle);
};

#endif // SCENE2_H
