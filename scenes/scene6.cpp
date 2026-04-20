#include "scene6.h"

#include <iostream>

Scene6::Scene6()
{
//    beam = Beam(-0.8660, -0.5, 0.5 *DropRadius, 550, DropRadius); // 120 degrees beam (30 to normal)
    beam = Beam(-1.19175, -1, DropRadius, 550, DropRadius); // 130 degrees beam (40 to normal)
    normal = Beam(1,0,0,550,DropRadius); // x=0
    refracted = beam;

    sceneScale = 10.0;

    offsetXFactor = 2;
    offsetYFactor = 1.7;

    scaleXFactor = 4.0;
    scaleYFactor = 4.0;

}

double Scene6::getCoordX(double x0)
{
    return x(x0);
}

double Scene6::getCoordY(double y0)
{
    return Y-y(DropRadius+y0);
}

void Scene6::drawWater()
{
    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(DropRadius));
    glVertex2f(x(-1000),y(DropRadius));
    glVertex2f(x(-1000),y(-1000));
    glVertex2f(x(1000),y(-1000));
    glVertex2f(x(1000),y(DropRadius));
    glEnd();
}

void Scene6::drawAxes()
{
    glColor3ub(255,255,255);
    glEnable(GL_LINE_STIPPLE); // turn on - - - - - -
    glLineStipple(1, 0x1111);  // 1 , 1111 means tiny dashes
    glBegin(GL_LINES);
    glVertex2f(x(-1000),y(DropRadius));
    glVertex2f(x(1000),y(DropRadius));
    glVertex2f(x(0),y(-1000));
    glVertex2f(x(0),y(1000));
    glEnd();
    glDisable(GL_LINE_STIPPLE); // turn it off
}

void Scene6::drawAngleArc(Beam &beam, double radius, bool arcOnTop)
{
    double a1 = beam.getAngle();
    double a2 = normal.getAngle();

    if(!arcOnTop) {
        a1 -= 180.0;
        a2 -= 180.0;
    }

    double ang1 = a1 * M_PI / 180.0;
    double ang2 = a2 * M_PI / 180.0;

    double d = ang2 - ang1;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= ImageQuality; i++)
    {
        double step = (double)i / ImageQuality;
        double ang = ang1 + d * step;

        double x0 = 0 + cos(ang) * radius;
        double y0 = DropRadius + sin(ang) * radius;

        glVertex2f(x(x0), y(y0));
    }
    glEnd();
}

void Scene6::rayProcess()
{
    double x0,y0,
           x1,y1,
           x2,y2;

    glColor3ub(255,255,255);

    beam.calculateInputPoint(&x0, &y0);
    beam.calculateInfinityPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    refracted = beam;
    refracted.snellIn(normal);
    refracted.calculateOutputPoint(x0, y0, &x1, &y1);
    refracted.calculateInfinityPoint(x1, y1, &x2, &y2);

    drawRay(x0,y0,x2,y2);
}

void Scene6::display()
{
    glLineWidth(5.0f);
    drawWater();
    drawAxes();
    rayProcess();
    drawAngleArc(beam, DropRadius, 1);
    drawAngleArc(refracted, DropRadius-0.5, 0);
    drawAngleArc(refracted, DropRadius+0.5, 0);
    glLineWidth(1.0f);
}
