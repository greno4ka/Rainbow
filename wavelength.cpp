#include "wavelength.h"
#include "beam.h"

void wavelengthToRGB(double wavelength, int *r, int *g, int *b) {
    const double Gamma = 0.80,
    IntensityMax = 255;

    double factor,
    red, green, blue;

    wavelength = trunc(wavelength);

    if ( (wavelength >= 380) && (wavelength < 440) ) {
        red = -(wavelength - 440) / (440 - 380);
        green = 0.0;
        blue = 1.0;
    } else if ( (wavelength >= 440) && (wavelength < 490) ) {
        red = 0.0;
        green = (wavelength - 440) / (490 - 440);
        blue = 1.0;
    } else if ( (wavelength >= 490) && (wavelength < 510) ) {
        red = 0.0;
        green = 1.0;
        blue = -(wavelength - 510) / (510 - 490);
    } else if ( (wavelength >= 510) && (wavelength < 580) ) {
        red = (wavelength - 510) / (580 - 510);
        green = 1.0;
        blue = 0.0;
    } else if ( (wavelength >= 580) && (wavelength < 645) ) {
        red = 1.0;
        green = -(wavelength - 645) / (645 - 580);
        blue = 0.0;
    } else if ( (wavelength >= 645) && (wavelength <= 780) ) {
        red = 1.0;
        green = 0.0;
        blue = 0.0;
    } else {
        red = 0.0;
        green = 0.0;
        blue = 0.0;
    }

    // Let the intensity fall off near the vision limits
    if ( (wavelength >= 380) && (wavelength < 420) ) {
        factor = 0.3 + 0.7*(wavelength - 380) / (420 - 380);
    } else if ( (wavelength >= 420) && (wavelength <= 700) ) {
        factor = 1.0;
    } else if ( (wavelength >= 701) && (wavelength <= 780) ) {
        factor = 0.3 + 0.7*(780 - wavelength) / (780 - 700);
    } else {
        factor = 0.0;
    }

    // Don't want 0^x = 1 for x <> 0
    red = red==0.0 ? 0 : (int) round(IntensityMax * pow(red * factor, Gamma));
    green = green==0.0 ? 0 : (int) round(IntensityMax * pow(green * factor, Gamma));
    blue = blue==0.0 ? 0 : (int) round(IntensityMax * pow(blue * factor, Gamma));

    if ( (wavelength < 380) || (wavelength > 780) ) {
        red = 255;
        green = 255;
        blue = 255;
    }

    // Return
    *r = red;
    *g = green;
    *b = blue;
}

double whatAngle(double wave, int rainbowMode)
{
    double direction,
           distance;
    double x0,y0,
           x1,y1;

    if (rainbowMode == 1) {
        direction = 1;
        distance = 0.87;
    } else {
        direction = -1;
        distance = 0.96;
    }

    /// use drop with radius 1 for example
    Beam input(0,1,-direction*distance,wave,1),
         refracted(1),
         radius(1);

    /// penetration of beam into drop
    input.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);
    refracted = radius;
    refracted.snell(input,input.refractIn());
    input = refracted;

    /// first reflection
    input.calculateOutputPoint(&x1, &y1, x0, y0);
    radius.calculateKoeffs(x1,y1,0,0);
    input.reflect(radius);
    x0=x1; y0=y1;

    input.calculateOutputPoint(&x1, &y1, x0, y0);
    radius.calculateKoeffs(x1,y1,0,0);
    if (rainbowMode == 1) {
        refracted = radius;
        refracted.snell(input,input.refractOut());
    } else {
        input.reflect(radius);
        x0=x1; y0=y1;

        input.calculateOutputPoint(&x1, &y1, x0, y0);
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snell(input,input.refractOut());
    }
    return refracted.getAngle();
}

/// Secant method
double func(double wave, double angle, int rainbowMode)
{
    return whatAngle(wave,rainbowMode)-angle;
}

double whatWave(double angle, int rainbowMode)
{
    double a=480,
           b=780,
           eps=1;
    while (fabs(b-a) > eps) {
        a = b - (b-a) * func(b,angle,rainbowMode) / (func(b,angle,rainbowMode) - func(a,angle,rainbowMode));
        b = a - (a-b) * func(a,angle,rainbowMode) / (func(a,angle,rainbowMode) - func(b,angle,rainbowMode));
    }
    return b;
}
