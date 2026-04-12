#ifndef SCENEX_H
#define SCENEX_H

#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>

class SceneX
{
protected:
    int X, Y; // Window height and width

public:
    SceneX();

    void updateXY(int newX, int newY);
    void display();
};

#endif // SCENEX_H
