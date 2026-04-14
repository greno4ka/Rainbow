#include "scene5.h"

#include "wavelength.h"

Scene5::Scene5()
{
    displayMode = 0;
    numberOfBeams = 30;
    reinitializeBeams();

    sceneScale = 20.0;

    offsetXFactor = 18.0;
    offsetYFactor = 8.0;

    scaleXFactor = 16.0;
    scaleYFactor = 8.0;
}

void Scene5::reinitializeBeams()
{
    beams.clear();

    double h=1.0/numberOfBeams;

    for (double r=0.01; r<0.99; r+=h)
    {
        for (int wavelength=400; wavelength<=760; wavelength+=(760-400)/7)
        {
            Beam beam(0, 1, -r*DropRadius, wavelength, DropRadius);
            beams.push_back(beam);
            beam.invertDistance();
            beams.push_back(beam);
        }
    }
    emit requestUpdate();
}

void Scene5::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene5::rayProcess(Beam beam)
{
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    int r,g,b;

    Beam refracted(DropRadius),
         radius(DropRadius),
         reflected(DropRadius);

    beam.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);

    glColor3ub(255,255,255);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    drawInitialRay(x0,y0);

    wavelengthToRGB(beam.getWavelength(),&r,&g,&b);
    if (beam.getDistance() > 0)
        glColor3ub(r,g,b);
    else
        glColor3ub(r*0.7,g*0.7,b*0.7);

    /// FIRST REFRACTION
    refracted = radius;
    refracted.snell(beam, beam.refractIn());
    beam = refracted;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    /// REFLECTION INSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    if ( displayMode == 0 || displayMode == 1) {
        /// REFRACTION OUTSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snell(beam, beam.refractOut());
        refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

        drawRay(x1,y1,x2,y2);
    }
    if ( displayMode == 0 || displayMode == 2) {
        /// NEXT REFLECTION INSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        beam.reflect(radius);
        x0=x1; y0=y1;
        beam.calculateOutputPoint(x0, y0, &x1, &y1);

        drawRay(x0,y0,x1,y1);

        /// REFRACTION OUTSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snell(beam, beam.refractOut());
        refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

        drawRay(x1,y1,x2,y2);
    }
}

void Scene5::display()
{
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        rayProcess(*beam);

    drawDrop();
}
