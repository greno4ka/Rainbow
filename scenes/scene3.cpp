#include "scene3.h"

Scene3::Scene3()
{
    resetScene();
}

void Scene3::resetScene()
{
    numberOfBeams = SCENE3_NUMBER_OF_BEAMS_30;
    wavelength = SCENE3_WAVELENGTH_600;
    displayMode = 1;
    showAngle = 0;
    showRainbow = 0;
    reinitializeBeams();
}

double Scene3::getCoordX()
{
    if (displayMode == 1)
        return x(coordX)+25;
    else
        return x(coordX)-100;

}

double Scene3::getCoordY()
{
    return Y-y(coordY)-5;
}

double Scene3::getCurrentAngle() const
{
    return currentAngle;
}

int Scene3::getShowAngle() const
{
    return showAngle;
}

void Scene3::reinitializeBeams()
{
    beams.clear();

    double h=1.0/numberOfBeams;

    for (double r=0.01; r<0.99; r+=h) {
        Beam beam(0,1,-r*DropRadius,wavelength,DropRadius);
        if (displayMode == 2) beam.invertDistance();
        beams.push_back(beam);
    }
    emit requestUpdate();
}

void Scene3::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene3::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
    reinitializeBeams();
}

void Scene3::setNumberOfBeams(int newNumberOfBeams)
{
    numberOfBeams = newNumberOfBeams;
    reinitializeBeams();
}

void Scene3::setShowRainbow(int newShowRainbow)
{
    showRainbow = newShowRainbow;
    emit requestUpdate();
}

void Scene3::setShowAngle(int newShowAngle)
{
    showAngle = newShowAngle;
    emit requestUpdate();
}

void Scene3::rayProcess(Beam beam)
{
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    int r,g,b;
    wavelengthToRGB(beam.getWavelength(),&r,&g,&b);
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

    Beam refracted,
         normal,
         reflected;

    beam.calculateInputPoint(&x0, &y0);
    normal = beam;
    normal.calculateCoeffs(x0,y0,0,0);

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

    if (displayMode == 1) {
        /// REFRACTION OUTSIDE
        normal.calculateCoeffs(x1,y1,0,0);
        refracted = beam;
        refracted.snellOut(normal);
        refracted.calculateInfinityPoint(x1,y1,&x2,&y2);

        if (showAngle && (originalBeam.getDistance() >= 0.84 && originalBeam.getDistance() <= 0.88) ) {
            Beam horizontal(0,1,2*DropRadius,0,DropRadius);
            cross_ll(refracted,horizontal,&coordX,&coordY);
            currentAngle = refracted.getAngle();

        }

        drawRay(x1,y1,x2,y2);
    } else {
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

        if (showAngle && (originalBeam.getDistance() <= -0.94 && originalBeam.getDistance() >= -0.97) ) {
            Beam horizontal(0,1,2*DropRadius,0,DropRadius);
            cross_ll(refracted,horizontal,&coordX,&coordY);
            currentAngle = refracted.getAngle();
        }

        drawRay(x1,y1,x2,y2);
    }

    if (showAngle) {
        glColor3ub(255,255,255);
        glBegin(GL_LINES);
        glVertex2f(x(coordX),y(coordY));
        if (displayMode == 1)
            glVertex2f(x(coordX)+100,y(coordY));
        else
            glVertex2f(x(coordX)-100,y(coordY));
        glEnd();
    }
}

void Scene3::display()
{
    drawDrop();
    drawAxes();
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        rayProcess(*beam);
}
