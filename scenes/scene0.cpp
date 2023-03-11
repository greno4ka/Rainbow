#include "scene0.h"

Scene0::Scene0()
{
    X = 800;
    Y = 600;
}

void Scene0::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

void Scene0::setDistance(double newDistance)
{
    distance = newDistance;
}

void Scene0::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
}

void Scene0::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}

void Scene0::incBeamStep()
{
    if (beamStep < MaxNumberOfReflections)
        beamStep++;
}

void Scene0::resetBeamStep()
{
    beamStep = 0;
}

void Scene0::decBeamStep()
{
    if (beamStep > 0)
    beamStep--;
}

inline double Scene0::x(double x0)
{
    return X-(X/4)+x0*std::min(X/4,Y/2)/SceneScale;
}

inline double Scene0::y(double y0)
{
    return (Y/2)+y0*std::min(X/4,Y/2)/SceneScale;
}

inline double Scene0::r(double r0)
{
    return r0*std::min(X/4,Y/2)/SceneScale;
}

void Scene0::draw_drop()
{
    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    const int DropQuality=50;

    for (int i=0; i<=DropQuality; i++)
    {
        double currentAngle = (double)i/DropQuality*2*M_PI;
        glVertex2f(x(0)+r(Scene0::DropRadius)*cos(currentAngle),
                   y(0)+r(Scene0::DropRadius)*sin(currentAngle));
    }
    glEnd();
}

void Scene0::draw_axes()
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

void Scene0::display()
{
    draw_drop();
    draw_axes();
}
