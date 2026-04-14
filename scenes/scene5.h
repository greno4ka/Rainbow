#ifndef SCENE5_H
#define SCENE5_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"
#include "wavelength.h"

class Scene5: public SceneBase
{
    /// DisplayMode values
    // 0 - show all beams
    // 1 - only 1st rainbow beams
    // 2 - only 2nd rainbow beams
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
