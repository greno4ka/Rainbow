#include "scene1.h"

#include "wavelength.h"

Scene1::Scene1()
{
    displayMode = 0;
    beamStep = 0;
}

void Scene1::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}

void Scene1::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
}

void Scene1::setDistance(double newDistance)
{
    distance = newDistance;
}

void Scene1::addBeam(double newDistance, int newWavelength)
{
    Beam newBeam(0, 1, -newDistance * DropRadius, newWavelength, DropRadius);
    beams.push_back(newBeam);
}

void Scene1::clearBeams()
{
    beams.clear();
    beamStep = 0;
}

bool Scene1::ifNoBeams()
{
    return beams.empty();
}

void Scene1::incBeamStep()
{
    if (beamStep < MaxNumberOfReflections)
        beamStep++;
}

void Scene1::decBeamStep()
{
    if (beamStep > 0)
        beamStep--;
}

void Scene1::draw_beam(Beam beam)
{
    double p=1;        // GAMMA CORRECTOR of color. For darkening beams
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    Beam refracted(DropRadius),
         radius(DropRadius),
         reflected(DropRadius);

    int r,g,b;
    wavelengthToRGB(beam.getWL(),&r,&g,&b);

    beam.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    glColor3ub(r*p,g*p,b*p);
    glBegin(GL_LINES);
    glVertex2f(0,y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();

    if (beamStep > 0) {
        /// FIRST REFLECTION
        // not shown in rainbows modes
        p -= 0.1; // low color intensity every beam split
        glColor3ub(r*p,g*p,b*p);

        if (displayMode == 0) {
            reflected = beam;
            reflected.reflect(radius);
            reflected.calculateInfinityPoint(&x2,&y2,x0,y0);

            glBegin(GL_LINES);
            glVertex2f(x(x0),y(y0));
            glVertex2f(x(x2),y(y2));
            glEnd();
        }

        /// FIRST REFRACTION
        refracted = radius; // we're get reformed from radius
        refracted.snell(beam, beam.refractIn());
        beam = refracted;
        beam.calculateOutputPoint(&x1, &y1, x0, y0);

        glBegin(GL_LINES);
        glVertex2f(x(x0),y(y0));
        glVertex2f(x(x1),y(y1));
        glEnd();

        for (int stepNumber=1; stepNumber < beamStep; stepNumber++)
        {
            /// REFRACTION OUTSIDE
            radius.calculateKoeffs(x1,y1,0,0);
            refracted = radius; // we're get reformed from radius again
            refracted.snell(beam, beam.refractOut());
            refracted.calculateInfinityPoint(&x2,&y2,x1,y1);

            p -= 0.1; // low color intensity every beam split
            glColor3ub(r*p,g*p,b*p);
            if ( (displayMode == 0) ||
                 ( (displayMode == 1) && (stepNumber == 2) ) ||
                 ( (displayMode == 2) && (stepNumber == 3) )
               ) {
                glBegin(GL_LINES);
                glVertex2f(x(x1),y(y1));
                glVertex2f(x(x2),y(y2));
                glEnd();
            }

            /// REFLECTION INSIDE
            beam.reflect(radius);
            x0=x1; y0=y1;
            beam.calculateOutputPoint(&x1, &y1, x0, y0);

            if ( (displayMode == 0) ||
                 ( (displayMode == 1) && (stepNumber == 1) ) ||
                 ( (displayMode == 2) && (stepNumber <= 2) )
               ) {
                glBegin(GL_LINES);
                glVertex2f(x(x0),y(y0));
                glVertex2f(x(x1),y(y1));
                glEnd();
            }
        }
    }
}

void Scene1::display()
{
    draw_drop();
    draw_axes();
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        draw_beam(*beam);
}
