#ifndef SCENEX_H
#define SCENEX_H

#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "beam.h"
#include "wavelength.h"

#include <QElapsedTimer>

#include <QVector3D>

typedef QList<QVector3D> QVector3Ds;

struct Vertex {
    QVector3D position;
    QVector3D color;

    Vertex() = default;

    Vertex(const QVector3D& pos, const QVector3D& col)
        : position(pos), color(col) {}
};

typedef QList<Vertex> Vertices;

class SceneX
{
    inline static const QVector3D SunColor = QVector3D(255, 255, 230);
    static const int ImageQuality = 100; // quality of drawing circles (drops, clouds...)
    static const int SunRadius = 200;
    static const int SunDistance = 10000;
    static const int EyeDistance = 100;

    static constexpr double AnimationDurability = 2.0; // 2 seconds

    QElapsedTimer timer;
    double rayAnimationStartTime = 0.0;

    QVector3D eyeCenter;
    QVector3D sunCenter;
    QVector3D raysDestination;
    QVector3D raysDirection;

    QVector3Ds sunPoints;
    QVector3Ds wallPoints;

    Vertices rainbowPoints;

    bool isPolychromatic;
    bool showBeams;


    double time = 0.0;


    int totalNumberOfBeams;
    void drawGrid();
    void drawSurface();
    void drawSun();
    void drawRay(QVector3D& startPoint, QVector3D& endPoint, QVector3D color);
    void drawSingleRay(const QVector3D& sunPoint,
                       const QVector3D& wallPoint,
                       const QVector3D color,
                       double timeSec);

    void drawWaterSpherePatch();
    void calculateSpherePoints();
    void drawSunGlow();
    void drawLensFlare();

    void drawSphere(const QVector3D& center, float radius, const QVector3D& color, int stacks, int slices);

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

    QVector3D getEye();


    void addBeams();
    void reInitializeScene();
    void resetScene();

    void updateXY(int newX, int newY);
    void display();
};

#endif // SCENEX_H
