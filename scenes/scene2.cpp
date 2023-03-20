#include "scene2.h"

#include "wavelength.h"

Scene2::Scene2()
{
    displayMode = 1;
    showAngle = 0;
    showRainbow = 0;
    wavelength = 600;
    numberOfBeams = 30;
    reinitializeBeams();
}

void Scene2::reinitializeBeams()
{
    beams.clear();

    double h=1.0/numberOfBeams;

    for (float r=0.01; r<0.99; r+=h)
    {
        Beam beam(0,1,-r*DropRadius,wavelength,DropRadius);
        if (displayMode == 2) beam.invertz();
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

    if (displayMode == 1)
        draw1stRainbow(beam);
    else
        draw2ndRainbow(beam);
}

void Scene2::display()
{
    draw_drop();
    draw_axes();
    for (Beams::iterator beam=beams.begin(); beam!=beams.end(); beam++)
        draw_beam(*beam);
}
