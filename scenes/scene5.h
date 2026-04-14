#ifndef SCENE5_H
#define SCENE5_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene5: public SceneBase
{
    int displayMode;
    int numberOfBeams;

    Beams beams;

    void reinitializeBeams();

    void rayProcess(Beam beam);

public:
    Scene5();

    void setDisplayMode(int newDisplayMode);

    void display();
};

#endif // SCENE5_H
