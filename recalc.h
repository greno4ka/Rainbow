#ifndef RECALC_H
#define RECALC_H

#include <cmath>

extern int X,Y;

template <class T> const T& min (const T& a, const T& b)
{
    return !(b<a)?a:b;     // or: return !comp(b,a)?a:b; for version (2)
}

inline double fye(double y0)
{
    double Z=min(X/8,Y/4);
    return (3*Y/4)+y0*Z/5;
}

inline double fxe(double x0)
{
    double Z=min(X/8,Y/4);
    return X-(X/8)+x0*Z/5;
}

inline double fre(double r0)
{
    double Z=min(X/8,Y/4);
    return r0*Z/5;
}

inline double fy4(double y0)
{
    double Z=min(X/8,Y/4);
    return (7*Y/8)+y0*Z/40;
}

inline double fx4(double x0)
{
    double Z=min(X/8,Y/4);
    return X-(X/16)+x0*Z/40;
}

inline double fr4(double r0)
{
    double Z=min(X/8,Y/4);
    return r0*Z/40;
}

#endif // RECALC_H
