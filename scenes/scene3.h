#ifndef SCENE3_H
#define SCENE3_H

#include <QOpenGLFunctions>

#include "scenebase.h"

#include "beam.h"

class Scene3 : public SceneBase
{
    int displayMode;
    bool dynamicMode;
    int desiredFPS;

public:
    Scene3();

    void setDisplayMode(int newDisplayMode);
    void setDesiredFPS(int newDesiredFPS);
    void switchDynamicMode();

    const int Scale=10;

    void draw_cloud(float w, float h, float R);
    void draw_floor(float ypos);
    void draw_man(float xpos, float ypos, float h);

    void display();

    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);
};

#endif // SCENE3_H
