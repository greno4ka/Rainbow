#include "scene0.h"

Scene0::Scene0()
{
    // double Z=std::min(X/4,Y/2);
}

double Scene0::getDistance() const
{
    return distance;
}

void Scene0::setDistance(double newDistance)
{
    distance = newDistance;
}

int Scene0::getWavelength() const
{
    return wavelength;
}

void Scene0::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
}

inline double Scene0::x(double x0)
{
    return X-(X/4)+x0*Z/Scale;
}

inline double Scene0::y(double y0)
{
    return (Y/2)+y0*Z/Scale;
}

inline double Scene0::r(double r0)
{
    return r0*Z/Scale;
}

void Scene0::draw_drop()
{
    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    const int DropQuality=50;

    for (int i=0; i<=DropQuality; i++)
    {
        double currentAngle = (float)i/(float)DropQuality*M_PI*2.0;
        glVertex2f(x(0)+r(R)*cos(currentAngle),y(0)+r(R)*sin(currentAngle));
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

}
