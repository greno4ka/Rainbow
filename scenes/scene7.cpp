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

int Scene7::getDisplaymode()
{
    return displayMode;
}

double Scene7::getCoordX(double x0)
{
    return x(x0);
}

double Scene7::getCoordY(double y0)
{
    return Y-y(DropRadius+y0);
}

void Scene7::drawAngleArc(Beam &beam1, Beam &beam2, double x0, double y0, double radius, bool arcOnTop)
{
    /// beam2 is usually a normal

    double a1 = beam1.getAngle();
    double a2 = beam2.getAngle();

        if(!arcOnTop) {
                a1 -= 180.0;
                a2 -= 180.0;
    }

    double ang1 = a1 * M_PI / 180.0;
    double ang2 = a2 * M_PI / 180.0;

    double d = ang2 - ang1;

    // force small arc
    if (fabs(d) > M_PI/2) {
        if (d > 0) d -= M_PI;
        else       d += M_PI;
    }

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= ImageQuality; i++) {
        double step = (double)i / ImageQuality;
        double ang = ang1 + d * step;

        double x1 = x0 + cos(ang) * radius;
        double y1 = y0 + sin(ang) * radius;

        glVertex2f(x(x1), y(y1));
    }
    glEnd();
}

void Scene7::drawRadiusDash(double x0, double y0)
{
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xFF00);
    glBegin(GL_LINES);
    glVertex2f(x(0),y(0));
    glVertex2f(x(x0),y(y0));
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void Scene7::rayProcess()
{
    Beam refracted,
         normal,
         reflected;

    if (displayMode == 0)
        beam = Beam(0, 1, -0.8 * DropRadius, 500, DropRadius);
    else
        beam = Beam(0, 1, 0.8 * DropRadius, 500, DropRadius);


    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)


    /// ORIGINAL BEAM
    beam.calculateInputPoint(&x0, &y0);
    normal = beam; // copy radius and !wavelength! attributes
    normal.calculateCoeffs(x0,y0,0,0);

    drawInitialRay(x0,y0);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(4, 0xFF00);
    glBegin(GL_LINES);
    glVertex2f(x(0),y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    normal.calculateInfinityPoint(x0,y0,&x2,&y2,3);

    drawRadiusDash(x2,y2);

    drawAngleArc(beam,normal,x0,y0,1,0);

    /// FIRST REFRACTION
    refracted = beam;
    refracted.snellIn(normal);
    beam = refracted;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    drawAngleArc(beam,normal,x0,y0,0.9,displayMode);
    drawAngleArc(beam,normal,x0,y0,1.1,displayMode);

    normal.calculateCoeffs(x1,y1,0,0);
    normal.calculateInfinityPoint(x1,y1,&x2,&y2,3);

    drawRadiusDash(x2,y2);

    drawAngleArc(beam,normal,x1,y1,0.9,!displayMode);
    drawAngleArc(beam,normal,x1,y1,1.1,!displayMode);

    /// REFLECTION INSIDE
    normal.calculateCoeffs(x1,y1,0,0);
    beam.reflect(normal);
    x0=x1;
    y0=y1;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    drawAngleArc(beam,normal,x0,y0,1.6,displayMode);
    drawAngleArc(beam,normal,x0,y0,1.4,displayMode);

    normal.calculateCoeffs(x1,y1,0,0);
    normal.calculateInfinityPoint(x1,y1,&x2,&y2,3);

    drawRadiusDash(x2,y2);

    drawAngleArc(beam,normal,x1,y1,1.6,!displayMode);
    drawAngleArc(beam,normal,x1,y1,1.4,!displayMode);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(4, 0xFF00);
    glBegin(GL_LINES);
    glVertex2f(0,y(y1));
    glVertex2f(x(x1),y(y1));
    glEnd();
    glDisable(GL_LINE_STIPPLE);


    if (displayMode == 1) { // second rainbow mode
        /// REFLECTION INSIDE
        normal.calculateCoeffs(x1,y1,0,0);
        beam.reflect(normal);
        x0=x1;
        y0=y1;
        beam.calculateOutputPoint(x0, y0, &x1, &y1);

        drawRay(x0,y0,x1,y1);

        drawAngleArc(beam,normal,x0,y0,0.9,!displayMode);
        drawAngleArc(beam,normal,x0,y0,1.1,!displayMode);

        normal.calculateCoeffs(x1,y1,0,0);
        normal.calculateInfinityPoint(x1,y1,&x2,&y2,3);

        drawRadiusDash(x2,y2);

        drawAngleArc(beam,normal,x1,y1,0.9,displayMode);
        drawAngleArc(beam,normal,x1,y1,1.1,displayMode);
    }

    /// REFRACTION OUTSIDE
    normal.calculateCoeffs(x1,y1,0,0);
    refracted = beam;
    refracted.snellOut(normal);
    refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

    drawRay(x1,y1,x2,y2);

    Beam tmp;
    Beam original(0, 1, 0.8 * DropRadius, 500, DropRadius);

    if (displayMode == 0)
    {
    tmp.calculateCoeffs(0,y1,x1,y1);
        drawAngleArc(tmp,refracted,x1,y1,1.7,displayMode);
        drawAngleArc(tmp,refracted,x1,y1,1.5,displayMode);
        drawAngleArc(tmp,refracted,x1,y1,1.3,displayMode);
    }
    else
    {
        cross_ll(original,refracted,&x2,&y2);
        drawAngleArc(original,refracted,x2,y2,0.8,!displayMode);
        drawAngleArc(original,refracted,x2,y2,1,!displayMode);
        drawAngleArc(original,refracted,x2,y2,1.2,!displayMode);

    }

}

void Scene7::display()
{
    glLineWidth(5.0f);
    drawDrop();
    drawAxes();
    rayProcess();
    glLineWidth(1.0f);
}
