#ifndef SCENEX_H
#define SCENEX_H

#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "beam.h"

#include <QElapsedTimer>

struct Vec3 {
    double x, y, z;
};

class SceneX
{
    Vec3 eyeCenter;
    Vec3 sunCenter;
    Vec3 wallCenter;
    Vec3 raysDirection;


    std::vector<Vec3> raysStart;
    std::vector<double> raysAngle;
    std::vector<double> raysRadius;

    QElapsedTimer timer;

    double time = 0.0;
    bool waiting = false;

    bool isPolychromatic;
    bool showBeams;

    int numberOfBeams;

    Beams beams;

    void drawWall();

    void drawAnimatedBeam(Beam beam, double t);

protected:
    int X, Y; // Window height and width


public:
    SceneX();

    void updateXY(int newX, int newY);
    void display();
};

#endif // SCENEX_H
