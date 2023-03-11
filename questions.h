#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <cmath>

#include "beam.h"

double what_angle(double wave, int var)
{
    double ug,     // nessesary distance
           dir,   // direction - depends on var
           x0,y0, // point0
           x1,y1, // point1
           x2,y2; // point2 - external (for reformed)
    if (var==1) // 1 or 2 - if 1 rainbow or 2
    {
        dir=1;
        ug=0.87;
    }
    else
    {
        dir=-1;
        ug=0.96;
    }

    Beam Input(0,1,-dir*ug*R,wave),
        Reformed,
        Rad; // Rad - Radius to the point1

    /// penetration of beam into drop
    Input.calculateInputPoint(&x0, &y0);
    Rad.calculateKoeffs(x0,y0,0,0);
    Reformed=Rad;                               // we're get reformed from radius
    Reformed.snell(Input,k(Input.getWL()));
    Input=Reformed;

    Input.calculateOutputPoint(&x1, &y1, x0, y0);
    Rad.calculateKoeffs(x1,y1,0,0);
    Input.reflect(Rad);
    x0=x1;
    y0=y1;

    Input.calculateOutputPoint(&x1, &y1, x0, y0);
    Rad.calculateKoeffs(x1,y1,0,0);
    if (var==1)
    {
        Reformed=Rad;                         // we're get reformed from radius again
        Reformed.snell(Input,1/k(Input.getWL())); // 1/k cause goin' from inside out
        Reformed.calculateInfintyPoint(&x2,&y2,x1,y1);
    }
    else
    {
        Input.reflect(Rad);
        x0=x1;
        y0=y1;

        Input.calculateOutputPoint(&x1, &y1, x0, y0);
        Rad.calculateKoeffs(x1,y1,0,0);
        Reformed=Rad;                         // we're get reformed from radius again
        Reformed.snell(Input,1/k(Input.getWL())); // 1/k cause goin' from inside out
        Reformed.calculateInfintyPoint(&x2,&y2,x1,y1);
    }
    return Reformed.getAngle();
}

double func(double wave, double angle, int var)
{
    return what_angle(wave,var)-angle;
}

double what_wave(double angle, int var)
{
    double a=480,b=780,
           eps=1;
    while(fabs(b-a)>eps)
    {
        a=b-(b-a)*func(b,angle,var)/(func(b,angle,var)-func(a,angle,var));
        b=a-(a-b)*func(a,angle,var)/(func(a,angle,var)-func(b,angle,var));
    }    // a - i-1, b - i-тый члены
    return b;
}

#endif // QUESTIONS_H
