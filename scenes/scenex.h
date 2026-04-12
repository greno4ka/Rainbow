#ifndef SCENEX_H
#define SCENEX_H

#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "beam.h"

class SceneX
{
    double eyeHeight;
    double sunHeight;

    bool isPolychromatic;
    bool showBeams;

    int numberOfBeams;

    Beams beams;

    void drawAnimatedBeam(Beam beam, double t);

protected:
    int X, Y; // Window height and width


public:
    SceneX();

    void updateXY(int newX, int newY);
    void display();
};

#endif // SCENEX_H
