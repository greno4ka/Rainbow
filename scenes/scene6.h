#ifndef SCENE6_H
#define SCENE6_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene6 : public SceneBase
{
    Beam beam;

    void drawWater();
    void drawAxes();
    void rayProcess();

public:
    Scene6();

    void display();
};

#endif // SCENE6_H
