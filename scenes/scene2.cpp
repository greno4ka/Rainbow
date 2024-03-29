#include "scene2.h"

#include "wavelength.h"

Scene2::Scene2()
{
    displayMode = 1;
    distance = 0.86;
    wavelength = 600;
    reinitializeBeams();
}

void Scene2::reinitializeBeams()
{
    beams.clear();
    Beam beam1(0,1,-(distance+0.02)*DropRadius,wavelength, DropRadius),
         beam2(0,1,-(distance-0.02)*DropRadius,wavelength, DropRadius);
    beams.push_back (beam1);
    beams.push_back (beam2);
}

void Scene2::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene2::setDistance(double newDistance)
{
    distance = newDistance;
    reinitializeBeams();
}

void Scene2::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
    reinitializeBeams();
}

void Scene2::draw_beam(Beam beam)
{
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    int r,g,b;
    wavelengthToRGB(beam.getWL(),&r,&g,&b);
    glColor3ub(r,g,b);

    Beam refracted(DropRadius),
         radius(DropRadius),
         reflected(DropRadius);

    beam.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    glBegin(GL_LINES);
    glVertex2f(0,y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();

    /// FIRST REFRACTION
    refracted = radius;
    refracted.snell(beam, beam.refractIn());
    beam = refracted;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    drawLine(x0,y0,x1,y1);

    /// REFLECTION INSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    drawLine(x0,y0,x1,y1);

    if (displayMode == 1) {
        /// REFRACTION OUTSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snell(beam, beam.refractOut());
        refracted.calculateInfinityPoint(&x2,&y2,x1,y1);

        drawLine(x1,y1,x2,y2);
    } else {
        /// NEXT REFLECTION INSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        beam.reflect(radius);
        x0=x1; y0=y1;
        beam.calculateOutputPoint(&x1, &y1, x0, y0);

        drawLine(x0,y0,x1,y1);

        /// REFRACTION OUTSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snell(beam, beam.refractOut());
        refracted.calculateInfinityPoint(&x2,&y2,x1,y1);

        drawLine(x1,y1,x2,y2);
    }
}

void Scene2::display()
{
    draw_drop();
    draw_axes();
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        draw_beam(*beam);
}
