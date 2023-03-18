#ifndef WAVELENGTH_H
#define WAVELENGTH_H

#include <cmath>

void wavelengthToRGB(double wavelength, int *r, int *g, int *b);

double whatAngle(double wave, int rainbowMode);
double whatWave(double angle, int rainbowMode);

#endif // WAVELENGTH_H
