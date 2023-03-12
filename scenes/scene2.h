#ifndef SCENE2_H
#define SCENE2_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene2 : public SceneBase
{
    int wavelength;
    int numberOfBeams;
    int displayMode;

public:
    Scene2();

    Lst Beams;

    void reinitializeBeams();

    void setWavelength(int newWavelength);
    void setNumberOfBeams(int newNumberOfBeams);
    void setDisplayMode(int newDisplayMode);

    void draw_beam(Beam beam);
    void display();

};

#endif // SCENE2_H
