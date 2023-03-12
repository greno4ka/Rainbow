#include "scene1.h"

#include "k.h"

Scene1::Scene1()
{
    displayMode = 1;
    distance = 0.86;
    wavelength = 600;
    reinitializeBeams();
}

void Scene1::reinitializeBeams()
{
    Beams.clear();
    Beam Input1(0,1,-(distance+0.02)*DropRadius,wavelength),
         Input2(0,1,-(distance-0.02)*DropRadius,wavelength);
    Beams.push_back (Input1);
    Beams.push_back (Input2);
}

void Scene1::setDistance(double newDistance)
{
    distance = newDistance;
    reinitializeBeams();
}

void Scene1::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
    reinitializeBeams();
}

void Scene1::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene1::draw_beam(Beam beam)
{
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)
    Beam refracted,
        radius,
        reflected;

    beam.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);

    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    glColor3ub(beam.getR(),beam.getG(),beam.getB());
    glBegin(GL_LINES);
    glVertex2f(0,y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();

    /// FIRST REFRACTION
    refracted = radius; // we're get reformed from radius
    refracted.snell(beam, k(beam.getWL()));
    beam = refracted;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();

    /// REFLECTION INSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();

    if (displayMode == 1)
    {
        /// REFRACTION OUTSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius; // we're get reformed from radius again
        refracted.snell(beam, 1/k(beam.getWL())); // 1/k because beam is going from inside out
        refracted.calculateInfintyPoint(&x2,&y2,x1,y1);

        glBegin(GL_LINES);
        glVertex2f(x(x1),y(y1));
        glVertex2f(x(x2),y(y2));
        glEnd();
    } else {
        /// REFLECTION INSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        beam.reflect(radius);
        x0=x1; y0=y1;
        beam.calculateOutputPoint(&x1, &y1, x0, y0);

        glBegin(GL_LINES);
        glVertex2f(x(x0),y(y0));
        glVertex2f(x(x1),y(y1));
        glEnd();

        /// REFRACTION OUTSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius; // we're get reformed from radius again
        refracted.snell(beam, 1/k(beam.getWL())); // 1/k because beam is going from inside out
        refracted.calculateInfintyPoint(&x2,&y2,x1,y1);

        glBegin(GL_LINES);
        glVertex2f(x(x1),y(y1));
        glVertex2f(x(x2),y(y2));
        glEnd();
    }

    // Disable antialising
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

void Scene1::display()
{
    draw_drop();
    draw_axes();
    for (Lst::iterator i=Beams.begin(); i!=Beams.end(); i++)
        draw_beam(*i);
}
