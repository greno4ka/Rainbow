#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <QOpenGLFunctions>

class SceneBase
{
    static const int DropQuality = 100;
    static const int SceneScale = 10;

protected:
    static const int DropRadius = 5;

    int X,Y; // Window height and width

    /// Methods of recalculation decart coords to screen coords
    double x(double x0);
    double y(double y0);
    double r(double r0);

    void draw_drop();
    void draw_axes();

public:
    SceneBase();

    void updateXY(int newX, int newY);
    virtual void display() = 0;
};

#endif // SCENEBASE_H
