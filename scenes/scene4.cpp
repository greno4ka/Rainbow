#include "scene4.h"

#include "wavelength.h"

Scene4::Scene4()
{
    displayMode = 0;
    numberOfBeams = 30;
    reinitializeBeams();
}

inline double Scene4::x(double x0)
{
    return X-(X/18)+x0*std::min(X/16,Y/8)/SceneScale/2;
}

inline double Scene4::y(double y0)
{
    return (7*Y/8)+y0*std::min(X/16,Y/8)/SceneScale/2;
}

inline double Scene4::r(double r0)
{
    return r0*std::min(X/16,Y/8)/SceneScale/2;
}

void Scene4::reinitializeBeams()
{
    beams.clear();

    double h=1.0/numberOfBeams;

    for (float r=0.01; r<0.99; r+=h)
    {
        for (int wavelength=400; wavelength<=760; wavelength+=(760-400)/7)
        {
            Beam beam(0, 1, -r*DropRadius, wavelength, DropRadius);
            beams.push_back(beam);
            beam.invertz();
            beams.push_back(beam);
        }
    }
}

void Scene4::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene4::draw_drop()
{
    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    for (int i=0; i<=ImageQuality; i++) {
        double currentAngle = (double)i/ImageQuality*2*M_PI;
        glVertex2f(x(0)+r(DropRadius)*cos(currentAngle),
                   y(0)+r(DropRadius)*sin(currentAngle));
    }
    glEnd();
}

void Scene4::drawLine(double x0, double y0, double x1, double y1)
{
    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();
}

void Scene4::draw_beam(Beam beam)
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
    glBegin(GL_LINES);
    glVertex2f(0,y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();

    wavelengthToRGB(beam.getWL(),&r,&g,&b);
    if (beam.getDistance() > 0)
        glColor3ub(r,g,b);
    else
        glColor3ub(r*0.7,g*0.7,b*0.7);

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

    if ( displayMode == 0 || displayMode == 1) {
        /// REFRACTION OUTSIDE
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snell(beam, beam.refractOut());
        refracted.calculateInfinityPoint(&x2,&y2,x1,y1);

        drawLine(x1,y1,x2,y2);
    }
    if ( displayMode == 0 || displayMode == 2) {
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

void Scene4::display()
{
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        draw_beam(*beam);

    draw_drop();
}
