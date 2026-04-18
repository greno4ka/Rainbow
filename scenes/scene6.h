#ifndef SCENE6_H
#define SCENE6_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene6 : public SceneBase
{
    Beam beam;

    void rayProcess(Beam beam);

public:
    Scene6();

    void display();
};

#endif // SCENE6_H
