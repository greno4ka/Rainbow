#ifndef SCENE3_H
#define SCENE3_H

#include <QOpenGLFunctions>

class Scene3
{
public:
    Scene3();

    const int Scale=10;
    int X,Y, // Window height and width
    Z,       // Coordinates center
    R;       // Radius of drop

    bool dynamic;

    void draw_cloud(float w, float h, float R);
    void draw_floor(float ypos);
    void draw_man(float xpos, float ypos, float h);

    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0);
    inline double y(double y0);
    inline double r(double r0);
};

#endif // SCENE3_H
