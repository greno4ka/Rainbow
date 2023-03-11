#include "scene1.h"

Scene1::Scene1()
{

}

void Scene1::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

void Scene1::setDistance(double newDistance)
{
    distance = newDistance;
}

void Scene1::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
}

void Scene1::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}
