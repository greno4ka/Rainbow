#include "scene7.h"

#include <iostream>

Scene7::Scene7()
{
    beam = Beam(-0.8660, -0.5, 0.5 *DropRadius, 550, DropRadius); // 120 degrees beam (30 to normal)
    normal = Beam(1,0,0,550,DropRadius); // x=0
    refracted = normal;

    sceneScale = 10.0;

    offsetXFactor = 2;
    offsetYFactor = 1.7;

    scaleXFactor = 4.0;
    scaleYFactor = 4.0;

}

double Scene7::getCoordX(double x0)
{
    return x(x0);
}

double Scene7::getCoordY(double y0)
{
    return Y-y(DropRadius+y0);
}

void Scene7::drawAngleArc(Beam &beam, double radius, bool arcOnTop)
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

void Scene7::rayProcess()
{
    double x0,y0,      // point0
        x1,y1,      // point1
        x2,y2;

    glColor3ub(255,255,255);

    beam.calculateInputPoint(&x0, &y0);
    beam.calculateInfinityPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    refracted = normal;
    refracted.snell(beam, beam.refractIn());
    refracted.calculateOutputPoint(x0, y0, &x1, &y1);
    refracted.calculateInfinityPoint(x1, y1, &x2, &y2);

    drawRay(x0,y0,x2,y2);
}

void Scene7::display()
{
    glLineWidth(5.0f);
    rayProcess();
    drawAngleArc(beam, DropRadius, 1);
    drawAngleArc(refracted, DropRadius-0.5, 0);
    drawAngleArc(refracted, DropRadius+0.5, 0);
}
