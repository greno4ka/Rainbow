#include "scene2.h"

#include "wavelength.h"

Scene2::Scene2()
{
    resetScene();
}

void Scene2::resetScene()
{
    displayMode = 1;
    showAngle = 0;
    showRainbow = 0;
    wavelength = 600;
    numberOfBeams = 30;
    reinitializeBeams();
}

double Scene2::getCoordX()
{
    if (displayMode == 1)
    return x(coordX)+100;
    else
        return x(coordX)-400;

}

double Scene2::getCoordY()
{
    return Y-y(coordY)-5;
}

double Scene2::getCurrentAngle() const
{
    return currentAngle;
}

int Scene2::getShowAngle() const
{
    return showAngle;
}

void Scene2::reinitializeBeams()
{
    beams.clear();

    double h=1.0/numberOfBeams;

    for (float r=0.01; r<0.99; r+=h)
    {
        Beam beam(0,1,-r*DropRadius,wavelength,DropRadius);
        if (displayMode == 2) beam.invertDistance();
        beams.push_back(beam);
    }
}

void Scene2::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene2::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
    reinitializeBeams();
}

void Scene2::setNumberOfBeams(int newNumberOfBeams)
{
    numberOfBeams = newNumberOfBeams;
    reinitializeBeams();
}

void Scene2::setShowRainbow(int newShowRainbow)
{
    showRainbow = newShowRainbow;
}

void Scene2::setShowAngle(int newShowAngle)
{
    showAngle = newShowAngle;
}

void Scene2::draw_beam(Beam beam)
{
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    int r,g,b;
    wavelengthToRGB(beam.getWL(),&r,&g,&b);
    glColor3ub(r,g,b);

    if (showRainbow)
        if (displayMode == 1) {
            if (!(beam.getDistance() >= 0.84 && beam.getDistance() <= 0.88))
                glColor3ub(r*0.5,g*0.5,b*0.5);
        } else {
            if (!(beam.getDistance() >= -0.97 && beam.getDistance() <= -0.93))
                glColor3ub(r*0.5,g*0.5,b*0.5);
        }

    Beam originalBeam = beam;

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

        if (showAngle && (originalBeam.getDistance() >= 0.84 && originalBeam.getDistance() <= 0.88) ) {
            Beam horizontal(0,1,2*DropRadius,0,DropRadius);
            cross_ll(refracted,horizontal,&coordX,&coordY);
            currentAngle = refracted.getAngle();

        }

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

    if (showAngle) {
    glColor3ub(255,255,255);
    glBegin(GL_LINES);
    glVertex2f(x(coordX),y(coordY));
    glVertex2f(x(coordX)+100,y(coordY));
    glEnd();
}
}

void Scene2::display()
{
    draw_drop();
    draw_axes();
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        draw_beam(*beam);
}
