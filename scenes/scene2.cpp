#include "scene2.h"

Scene2::Scene2()
{
    displayMode = 1;
    distance = SCENE2_DISTANCE_086;
    wavelength = SCENE2_WAVELENGTH_600;
    reinitializeBeams();
}

void Scene2::reinitializeBeams()
{
    beams.clear();
    Beam beam1(0,1,-(distance+0.02)*DropRadius,wavelength, DropRadius),
         beam2(0,1,-(distance-0.02)*DropRadius,wavelength, DropRadius);
    beams.push_back (beam1);
    beams.push_back (beam2);
    emit requestUpdate();
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

void Scene2::rayProcess(Beam beam)
{
    Beam refracted,
         normal,
         reflected;

    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    int r,g,b;

    beam.calculateInputPoint(&x0, &y0);
    normal = beam; // copy radius and !wavelength! attributes
    normal.calculateCoeffs(x0,y0,0,0);

    /// Single color for all rays
    wavelengthToRGB(beam.getWavelength(),&r,&g,&b);
    glColor3ub(r,g,b);
    /// ORIGINAL BEAM
    // this part should be drawn anyway
    drawInitialRay(x0,y0);

    /// FIRST REFRACTION
    refracted = beam;
    refracted.snellIn(normal);
    beam = refracted;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    /// REFLECTION INSIDE
    normal.calculateCoeffs(x1,y1,0,0);
    beam.reflect(normal);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    if (displayMode == 1) { // 1st rainbow
        /// REFRACTION OUTSIDE
        normal.calculateCoeffs(x1,y1,0,0);
        refracted = beam;
        refracted.snellOut(normal);
        refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

        drawRay(x1,y1,x2,y2);
    } else {                // 2nd rainbow
        /// NEXT REFLECTION INSIDE
        normal.calculateCoeffs(x1,y1,0,0);
        beam.reflect(normal);
        x0=x1; y0=y1;
        beam.calculateOutputPoint(x0, y0, &x1, &y1);

        drawRay(x0,y0,x1,y1);

        /// REFRACTION OUTSIDE
        normal.calculateCoeffs(x1,y1,0,0);
        refracted = beam;
        refracted.snellOut(normal);
        refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

        drawRay(x1,y1,x2,y2);
    }
}

void Scene2::display()
{
    glLineWidth(dpiScale);
    drawDrop();
    drawAxes();
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        rayProcess(*beam);
}
