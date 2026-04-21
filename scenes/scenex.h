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
    static const int SunRadius = 10;
    static const int SunDistance = 100;
    static const int EyeDistance = 10;

    QVector3D eyeCenter;
    QVector3D sunCenter;
    QVector3D wallCenter;
    QVector3D raysDirection;

    QVector3Ds sunPoints;
    QVector3Ds wallPoints;

    bool isPolychromatic;
    bool showBeams;

    QElapsedTimer timer;

    double time = 0.0;


    int totalNumberOfBeams;
    void drawSun();
    void drawWall();
    void drawSingleRay(const QVector3D& sunPoint,
                       const QVector3D& wallPoint,
                       const QVector3D color,
                       double timeSec);

protected:
    int X, Y; // Window height and width


public:
    SceneX();

    QVector3D generateSunPoint();
    void addSunPoints(int numberOfPoints);
    void calculateWallPoints();
    double calculateAngle(QVector3D& sunPoint, QVector3D& wallPoint, QVector3D& eyePoint);

    void setShowBeams(int newShowBeams);
    void setPolychromatic(bool newPolychromatic);
    void addNumberOfBeams(int newNumberOfBeams);
    void setSunHeight(int newSunHeight);
    void setEyeHeight(int newEyeHeight);

    void drawSphere(const QVector3D& c,
                            float r,
                            const QVector3D& color);

    void addBeams();
    void reInitializeScene();
    void resetScene();

    void updateXY(int newX, int newY);
    void display();
};

#endif // SCENEX_H
