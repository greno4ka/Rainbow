#include "scene0.h"

#include "wavelength.h"

Scene0::Scene0()
{
    displayMode = 0;
    beamStep = 0;
}

void Scene0::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}

void Scene0::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
}

void Scene0::setDistance(double newDistance)
{
    distance = newDistance;
}

void Scene0::addBeam(double newDistance, int newWavelength)
{
    Beam newBeam(0, 1, -newDistance * DropRadius, newWavelength, DropRadius);
    beams.push_back(newBeam);
}

void Scene0::clearBeams()
{
    beams.clear();
}

bool Scene0::ifNoBeams()
{
    return beams.empty();
}

void Scene0::incBeamStep()
{
    if (beamStep < MaxNumberOfReflections)
        beamStep++;
}

void Scene0::decBeamStep()
{
    if (beamStep > 0)
    beamStep--;
}

void Scene0::resetBeamStep()
{
    beamStep = 0;
}

void Scene0::draw_beam(Beam beam)
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

    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

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

    // Disable antialising
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

void Scene0::display()
{
    draw_drop();
    draw_axes();
    for (Beams::iterator i=beams.begin(); i!=beams.end(); i++)
        draw_beam(*i);
}
