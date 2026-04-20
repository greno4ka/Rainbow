#include "scene1.h"

Scene1::Scene1()
{
    displayMode = 0;
    beamStep = 0;
}

void Scene1::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    emit requestUpdate();
}

void Scene1::addBeam(double newDistance, int newWavelength)
{
    Beam newBeam(0, 1, -newDistance * DropRadius, newWavelength, DropRadius);
    beams.push_back(newBeam);
    emit requestUpdate();
}

void Scene1::clearBeams()
{
    beams.clear();
    beamStep = 0;
    emit requestUpdate();
}

bool Scene1::ifNoBeams()
{
    return beams.empty();
}

void Scene1::incBeamStep()
{
    if (beamStep < MaxNumberOfReflections)
        beamStep++;
    emit requestUpdate();
}

void Scene1::decBeamStep()
{
    if (beamStep > 0)
        beamStep--;
    emit requestUpdate();
}

void Scene1::rayProcess(Beam beam)
{
    // Here we define normal with setted radius
    // so further we can calculate coeffs just with 4 points
    // refracted and reflect will be initialized by copying
    Beam normal(DropRadius),
         refracted,
         reflected;

    double p=1;        // GAMMA CORRECTOR of color for darkening beams
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    int r,g,b;

    beam.calculateInputPoint(&x0, &y0);
    normal.calculateKoeffs(x0,y0,0,0);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    wavelengthToRGB(beam.getWavelength(),&r,&g,&b);
    glColor3ub(r*p,g*p,b*p);
    drawInitialRay(x0,y0);

    if (beamStep > 0) {
        /// FIRST REFLECTION
        // not shown in rainbows modes
        p -= 0.1; // low color intensity every beam split
        glColor3ub(r*p,g*p,b*p);

        if (displayMode == 0) { // only when displaying ALL
            reflected = beam;
            reflected.reflect(normal);
            reflected.calculateInfinityPoint(x0,y0,&x2,&y2);

            drawRay(x0,y0,x2,y2);
        }

        /// FIRST REFRACTION
        refracted = normal; // we're get reformed from radius
        refracted.snell(beam, beam.refractIn());
        beam = refracted;
        beam.calculateOutputPoint(x0, y0, &x1, &y1);

        drawRay(x0,y0,x1,y1);

        for (int stepNumber=1; stepNumber < beamStep; stepNumber++) {
            p -= 0.1; // low color intensity every beam split
            glColor3ub(r*p,g*p,b*p);

            /// REFRACTION OUTSIDE
            normal.calculateKoeffs(x1,y1,0,0);
            refracted = normal; // we're get reformed from radius again
            refracted.snell(beam, beam.refractOut());
            refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

            if ( (displayMode == 0) ||                          // ALL
                 ( (displayMode == 1) && (stepNumber == 2) ) || // 1st rainbow
                 ( (displayMode == 2) && (stepNumber == 3) )    // 2nd rainbow
               ) {

                drawRay(x1,y1,x2,y2);
            }

            /// REFLECTION INSIDE
            beam.reflect(normal);
            x0=x1; y0=y1;
            beam.calculateOutputPoint(x0, y0, &x1, &y1);

            if ( (displayMode == 0) ||                          // ALL
                 ( (displayMode == 1) && (stepNumber == 1) ) || // 1st rainbow
                 ( (displayMode == 2) && (stepNumber <= 2) )    // 2nd rainbow
               ) {
                drawRay(x0,y0,x1,y1);
            }
        }
    }
}

void Scene1::display()
{
    drawDrop();
    drawAxes();
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        rayProcess(*beam);
}
