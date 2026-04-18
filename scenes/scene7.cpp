#include "scene7.h"

#include <iostream>

Scene7::Scene7()
{
    normal = Beam(1,0,0,550,DropRadius); // x=0

    displayMode = 0;

    sceneScale = 3.5;

    offsetXFactor = 2.5;
    offsetYFactor = 1.9;

    scaleXFactor = 4.0;
    scaleYFactor = 4.0;

}


void Scene7::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
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
    for (int i = 0; i <= ImageQuality; i++) {
        double step = (double)i / ImageQuality;
        double ang = ang1 + d * step;

        double x0 = 0 + cos(ang) * radius;
        double y0 = DropRadius + sin(ang) * radius;

        glVertex2f(x(x0), y(y0));
    }
    glEnd();
}

void Scene7::drawRadiusDash(double x0, double y0)
{
    glEnable(GL_LINE_STIPPLE); // turn on - - - - - -
    glLineStipple(2, 0xFF00);  // 1 , 1111 means tiny dashes
    glBegin(GL_LINES);
    glVertex2f(x(0),y(0));
    glVertex2f(x(x0),y(y0));
    glEnd();
    glDisable(GL_LINE_STIPPLE); // turn it off
}

void Scene7::rayProcess()
{
    Beam refracted(DropRadius),
         radius(DropRadius),
         reflected(DropRadius);

    if (displayMode == 0)
        beam = Beam(0, 1, -0.8 * DropRadius, 500, DropRadius);
    else
        beam = Beam(0, 1, 0.8 * DropRadius, 500, DropRadius);


    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    /// ORIGINAL BEAM
    beam.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);

    drawInitialRay(x0,y0);

    radius.calculateInfinityPoint(x0,y0,&x2,&y2,3);

    drawRadiusDash(x2,y2);

    /// FIRST REFRACTION
    refracted = radius; // we're get reformed from radius
    refracted.snell(beam, beam.refractIn());
    beam = refracted;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    radius.calculateKoeffs(x1,y1,0,0);
    radius.calculateInfinityPoint(x1,y1,&x2,&y2,3);

    drawRadiusDash(x2,y2);

    /// REFLECTION INSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1;
    y0=y1;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    radius.calculateKoeffs(x1,y1,0,0);
    radius.calculateInfinityPoint(x1,y1,&x2,&y2,3);

    drawRadiusDash(x2,y2);

    if (displayMode == 1) { // second rainbow mode
        /// REFLECTION INSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        beam.reflect(radius);
        x0=x1;
        y0=y1;
        beam.calculateOutputPoint(x0, y0, &x1, &y1);

        drawRay(x0,y0,x1,y1);

        radius.calculateKoeffs(x1,y1,0,0);
        radius.calculateInfinityPoint(x1,y1,&x2,&y2,3);

        drawRadiusDash(x2,y2);
    }

    /// REFRACTION OUTSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    refracted = radius; // we're get reformed from radius again
    refracted.snell(beam, beam.refractOut());
    refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

    drawRay(x1,y1,x2,y2);

}

void Scene7::display()
{
    glLineWidth(5.0f);
    drawDrop();
    drawAxes();
    rayProcess();
//   drawAngleArc(beam, DropRadius, 1);
//    drawAngleArc(refracted, DropRadius-0.5, 0);
//    drawAngleArc(refracted, DropRadius+0.5, 0);
}
