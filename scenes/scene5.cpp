#include "scene5.h"

Scene5::Scene5()
{
    numberOfBeams = SCENE5_NUMBER_OF_BEAMS;
    beamQuality = SCENE5_BEAM_QUALITY_10;
    displayMode = 0;

    /// Scene5 is the only scene with special coordinates
    // redefine SceneBase properties
    sceneScale = 20.0;

    offsetXFactor = 18.0;
    offsetYFactor = 8.0;

    scaleXFactor = 16.0;
    scaleYFactor = 8.0;

    reinitializeBeams();
}

void Scene5::reinitializeBeams()
{
    beams.clear();

    double h=1.0/(numberOfBeams+1);

    for (double r=0.01; r<0.99; r+=h) {
        for (int wavelength=400; wavelength<=760; wavelength+=(760-400)/beamQuality) {
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

void Scene5::setNumberOfBeams(int newNumberOfBeams)
{
    numberOfBeams = newNumberOfBeams;
    reinitializeBeams();
}

void Scene5::setBeamQuality(int newBeamQuality)
{
    beamQuality = newBeamQuality;
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
    radius.calculateCoeffs(x0,y0,0,0);

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
    refracted.snellIn(beam);
    beam = refracted;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    /// REFLECTION INSIDE
    radius.calculateCoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(x0, y0, &x1, &y1);

    drawRay(x0,y0,x1,y1);

    if ( displayMode == 0 || displayMode == 1) {
        /// REFRACTION OUTSIDE
        radius.calculateCoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snellOut(beam);
        refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

        drawRay(x1,y1,x2,y2);
    }
    if ( displayMode == 0 || displayMode == 2) {
        /// NEXT REFLECTION INSIDE
        radius.calculateCoeffs(x1,y1,0,0);
        beam.reflect(radius);
        x0=x1; y0=y1;
        beam.calculateOutputPoint(x0, y0, &x1, &y1);

        drawRay(x0,y0,x1,y1);

        /// REFRACTION OUTSIDE
        radius.calculateCoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snellOut(beam);
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
