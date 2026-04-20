#ifndef SCENEX_H
#define SCENEX_H

#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "beam.h"
#include "wavelength.h"

#include <QElapsedTimer>

#include <QVector3D>

typedef QList<QVector3D> QVector3Ds;

class SceneX
{
    inline static const QVector3D SunColor = QVector3D(255, 255, 230);
    static const int ImageQuality = 100; // quality of drawing circles (drops, clouds...)

    QVector3D eyeCenter;
    QVector3D sunCenter;
    QVector3D wallCenter;
    QVector3D raysDirection;

    QVector3Ds sunPoints;
    QVector3Ds wallPoints;

    Beams beams;


    double sunRadius;

    QElapsedTimer timer;

    double time = 0.0;
    bool waiting = false;

    bool isPolychromatic;
    bool showBeams;

    int numberOfBeams;
    void drawSun();
    void drawWall();
    void drawSingleRay(const QVector3D& sunPoint,
                       const QVector3D& wallPoint,
                       const QVector3D color,
                       double timeSec);
    void drawAnimatedBeam(Beam beam, double t);

protected:
    int X, Y; // Window height and width


public:
    SceneX();

    QVector3D generateSunPoint();
    void addSunPoints(int numberOfPoints);
    void calculateWallPoints();
    double calculateAngle(QVector3D& sunPoint, QVector3D& wallPoint, QVector3D& eyePoint);

    void updateXY(int newX, int newY);
    void display();
};

#endif // SCENEX_H
