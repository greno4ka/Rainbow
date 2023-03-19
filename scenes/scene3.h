#ifndef SCENE3_H
#define SCENE3_H

#include <QOpenGLFunctions>

#include "scenebase.h"

class Scene3 : public SceneBase
{
    int displayMode;
    bool dynamicMode;
    int desiredFPS;

    // used for floor and man
    const double ManPositionX = -5,
                 ManPositionY = -11,
                 ManHeight = 3;

public:
    Scene3();

    void setDisplayMode(int newDisplayMode);
    void setDesiredFPS(int newDesiredFPS);
    void switchDynamicMode();

    const int Scale=10;

    void drawCloud();
    void drawFloor();
    void drawMan();

    void display();

    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);
};

#endif // SCENE3_H
