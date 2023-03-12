#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <QOpenGLFunctions>

class SceneBase
{
protected:
    static const int SceneScale = 10;
    static const int DropRadius = 5;
    int X,Y; // Window height and width

public:
    SceneBase();

    void updateXY(int newX, int newY);

    /// Methods of recalculation decart coords to screen coords
    double x(double x0);
    double y(double y0);
    double r(double r0);

    void draw_drop();
    void draw_axes();
};

#endif // SCENEBASE_H
