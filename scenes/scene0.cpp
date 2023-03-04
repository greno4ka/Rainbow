#include "scene0.h"

Scene0::Scene0()
{
    // double Z=std::min(X/4,Y/2);
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
