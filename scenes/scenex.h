#ifndef SCENEX_H
#define SCENEX_H

#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "beam.h"

#include <QElapsedTimer>

#include <QVector3D>

typedef QList<QVector3D> QVector3Ds;

class SceneX
{
    QVector3D eyeCenter;
    QVector3D sunCenter;
    QVector3D wallCenter;
    QVector3D raysDirection;

    QVector3Ds sunPoints;
    QVector3Ds wallPoints;


    double sunRadius;

    QElapsedTimer timer;

    double time = 0.0;
    bool waiting = false;

    bool isPolychromatic;
    bool showBeams;

    int numberOfBeams;

    Beams beams;

    void drawSun(double cx, double cy, double cz);
    void drawWall();
    void drawSingleRay(const QVector3D& sunPoint,
                               const QVector3D& wallPoint,
                               double timeSec);
    void drawAnimatedBeam(Beam beam, double t);

protected:
    int X, Y; // Window height and width


public:
    SceneX();

    QVector3D generateSunPoint();
    void addSunPoints(int numberOfPoints);
    void calculateWallPoints();

    void updateXY(int newX, int newY);
    void display();
};

#endif // SCENEX_H
