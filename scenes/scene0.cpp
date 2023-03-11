#include "scene0.h"

#include "k.h"

Scene0::Scene0()
{
    X = 800;
    Y = 600;
    beamStep = 0;
}

void Scene0::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

void Scene0::setDistance(double newDistance)
{
    distance = newDistance;
}

void Scene0::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
}

void Scene0::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}

void Scene0::incBeamStep()
{
    if (beamStep < MaxNumberOfReflections)
        beamStep++;
}

void Scene0::resetBeamStep()
{
    beamStep = 0;
}

void Scene0::decBeamStep()
{
    if (beamStep > 0)
    beamStep--;
}

inline double Scene0::x(double x0)
{
    return X-(X/4)+x0*std::min(X/4,Y/2)/SceneScale;
}

inline double Scene0::y(double y0)
{
    return (Y/2)+y0*std::min(X/4,Y/2)/SceneScale;
}

inline double Scene0::r(double r0)
{
    return r0*std::min(X/4,Y/2)/SceneScale;
}

void Scene0::draw_drop()
{
    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    const int DropQuality=50;

    for (int i=0; i<=DropQuality; i++)
    {
        double currentAngle = (double)i/DropQuality*2*M_PI;
        glVertex2f(x(0)+r(Scene0::DropRadius)*cos(currentAngle),
                   y(0)+r(Scene0::DropRadius)*sin(currentAngle));
    }
    glEnd();
}

void Scene0::draw_axes()
{
    glColor3ub(255,255,255);
    glEnable(GL_LINE_STIPPLE); // turn on - - - - - -
    glLineStipple(1, 0x1111);  // 1 , 1111 means tiny dashes
    glBegin(GL_LINES);
    glVertex2f(0,y(0));
    glVertex2f(X,y(0));
    glVertex2f(x(0),0);
    glVertex2f(x(0),Y);
    glEnd();
    glDisable(GL_LINE_STIPPLE); // turn it off
}

void Scene0::draw_beam(Beam beam)
{
    double p=1,     // GAMMA CORRECTOR of color. For darkening beams
        x0,y0,      // point0
        x1,y1,      // point1
        x2,y2;      // point2 - external (for reformed outside)
    int r,g,b;      // color of beam
    Beam refracted,
        radius,
        reflected;

    // Save original beam color before multiple transformations
    r = beam.getR();
    g = beam.getG();
    b = beam.getB();

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
        if (displayMode == 0) {
            reflected = beam;
            reflected.reflect(radius);
            reflected.calculateInfintyPoint(&x2,&y2,x0,y0);

            glColor3ub(r*p,g*p,b*p);
            glBegin(GL_LINES);
            glVertex2f(x(x0),y(y0));
            glVertex2f(x(x2),y(y2));
            glEnd();
        }

        /// FIRST REFRACTION
        refracted = radius; // we're get reformed from radius
        refracted.snell(beam, k(beam.getWL()));
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
            refracted.snell(beam, 1/k(beam.getWL())); // 1/k because beam is going from inside out
            refracted.calculateInfintyPoint(&x2,&y2,x1,y1);

            p -= 0.1; // low color intensity every beam split
            if ( (displayMode == 0) ||
               ( (displayMode == 1) && (stepNumber == 2) ) ||
               ( (displayMode == 2) && (stepNumber == 3) )
               ) {
                glColor3ub(r*p,g*p,b*p);
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
    for (Lst::iterator i=Beams.begin(); i!=Beams.end(); i++)
        draw_beam(*i);
}
