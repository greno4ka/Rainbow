#include "scenebase.h"

SceneBase::SceneBase()
{
    X = 800;
    Y = 600;
}

void SceneBase::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

double SceneBase::x(double x0)
{
    return X-(X/4)+x0*std::min(X/4,Y/2)/SceneScale;
}

double SceneBase::y(double y0)
{
    return (Y/2)+y0*std::min(X/4,Y/2)/SceneScale;
}

double SceneBase::r(double r0)
{
    return r0*std::min(X/4,Y/2)/SceneScale;
}

void SceneBase::draw_drop()
{
    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);

    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    for (int i=0; i<=DropQuality; i++) {
        double currentAngle = (double)i/DropQuality*2*M_PI;
        glVertex2f(x(0)+r(DropRadius)*cos(currentAngle),
                   y(0)+r(DropRadius)*sin(currentAngle));
    }
    glEnd();

    // Disable antialising
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
}

void SceneBase::draw_axes()
{
    glColor3ub(255,255,255);
    glEnable(GL_LINE_STIPPLE); // turn on - - - - - -
    glLineStipple(1, 0x1111);  // 1 , 1111 means tiny dashes
    glBegin(GL_LINES);
    glVertex2f(0,y(0));
    glVertex2f(X,y(0));
    glVertex2f(x(0),0);
    glVertex2f(x(0),Y);
    glEnd();
    glDisable(GL_LINE_STIPPLE); // turn it off
}
