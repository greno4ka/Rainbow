#include "scene1.h"

Scene1::Scene1()
{
    displayMode = 1;
    distance = 0.86;
    wavelength = 600;
    reinitializeBeams();
}

void Scene1::reinitializeBeams()
{
    beams.clear();
    Beam beam1(0,1,-(distance+0.02)*DropRadius,wavelength, DropRadius),
         beam2(0,1,-(distance-0.02)*DropRadius,wavelength, DropRadius);
    beams.push_back (beam1);
    beams.push_back (beam2);
}

void Scene1::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene1::setDistance(double newDistance)
{
    distance = newDistance;
    reinitializeBeams();
}

void Scene1::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
    reinitializeBeams();
}

void Scene1::draw_beam(Beam beam)
{
    if (displayMode == 1)
        draw1stRainbow(beam);
    else
        draw2ndRainbow(beam);
}

void Scene1::display()
{
    draw_drop();
    draw_axes();
    for (Beams::iterator i=beams.begin(); i!=beams.end(); i++)
        draw_beam(*i);
}
