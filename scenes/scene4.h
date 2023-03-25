#ifndef SCENE4_H
#define SCENE4_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene4: public SceneBase
{
    int displayMode;
    int numberOfBeams;

    Beams beams;

    void reinitializeBeams();

    void drawLine(double x0, double y0, double x1, double y1);
    void draw_beam(Beam beam);
    void draw_drop();

public:
    Scene4();

    void setDisplayMode(int newDisplayMode);

    void display();

    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);
};

#endif // SCENE4_H
