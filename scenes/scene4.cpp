#include "scene4.h"

#include "wavelength.h"

Scene4::Scene4()
{
    displayMode = 0;
    numberOfBeams = 30;
    reinitializeBeams();
}

void Scene4::reinitializeBeams()
{
    beams1.clear();
    beams2.clear();

    double h=1.0/numberOfBeams;

    for (float r=0.01; r<0.99; r+=h)
    {
        for (int wavelength=400; wavelength<=760; wavelength+=(760-400)/7)
        {
            Beam beam(0, 1, -r*DropRadius, wavelength, DropRadius);
            beams1.push_back(beam);
            beam.invertz();
            beams2.push_back(beam);
        }
    }
}

void Scene4::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
    reinitializeBeams();
}

void Scene4::display()
{
    draw_drop();
    draw_axes();

    int r,g,b;
    switch (displayMode) {
    case 0:
        for (Beams::iterator beam=beams1.begin(); beam!=beams1.end(); beam++) {
            wavelengthToRGB(beam->getWL(),&r,&g,&b);
            glColor3ub(r,g,b);
            draw1stRainbow(*beam);
        }
        for (Beams::iterator beam=beams2.begin(); beam!=beams2.end(); beam++) {
            wavelengthToRGB(beam->getWL(),&r,&g,&b);
            glColor3ub(r,g,b);
            draw2ndRainbow(*beam);
        }
        break;
    case 1:
        for (Beams::iterator beam=beams1.begin(); beam!=beams1.end(); beam++) {
            wavelengthToRGB(beam->getWL(),&r,&g,&b);
            glColor3ub(r,g,b);
            draw1stRainbow(*beam);
        }
        break;
    case 2:
        for (Beams::iterator beam=beams2.begin(); beam!=beams2.end(); beam++) {
            wavelengthToRGB(beam->getWL(),&r,&g,&b);
            glColor3ub(r,g,b);
            draw2ndRainbow(*beam);
        }
        break;
    default:
        break;
    }
}
