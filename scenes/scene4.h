#ifndef SCENE4_H
#define SCENE4_H

#include <QOpenGLFunctions>

#include "scenebase.h"

class Scene4: public SceneBase
{
    int displayMode;
    int numberOfBeams;

    Beams beams1, beams2;

    void reinitializeBeams();
public:
    Scene4();

    void setDisplayMode(int newDisplayMode);

    void display();
};

#endif // SCENE4_H
