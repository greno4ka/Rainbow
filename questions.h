#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <cmath>

#include "beam.h"
#include "k.h"

double what_angle(double wave, int rainbowMode)
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

    Beam input(0,1,-direction*distance,wave,1),
         refracted(1),
         radius(1);

    /// penetration of beam into drop
    input.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);
    refracted = radius;
    refracted.snell(input,k(input.getWL()));
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
        refracted.snell(input,1/k(input.getWL()));
    } else {
        input.reflect(radius);
        x0=x1; y0=y1;

        input.calculateOutputPoint(&x1, &y1, x0, y0);
        radius.calculateKoeffs(x1,y1,0,0);
        refracted = radius;
        refracted.snell(input,1/k(input.getWL()));
    }
    return refracted.getAngle();
}

/// Secant method
double func(double wave, double angle, int var)
{
    return what_angle(wave,var)-angle;
}

double what_wave(double angle, int var)
{
    double a=480,
           b=780,
           eps=1;
    while (fabs(b-a) > eps) {
        a = b - (b-a) * func(b,angle,var) / (func(b,angle,var) - func(a,angle,var));
        b = a - (a-b) * func(a,angle,var) / (func(a,angle,var) - func(b,angle,var));
    }
    return b;
}

#endif // QUESTIONS_H
