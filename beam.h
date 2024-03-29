#ifndef BEAM_H
#define BEAM_H

#include <QList>

#include <cmath>

class Beam
{
    double a,b,c,       // koefficients in equation ax+y+c=0
    phi,                // positive angle of beam line
                        // it's physical simulation, so we need angle
                        // only in physical sense, not in mathematical
    wavelength,
    r;                  // single beam interacts with single drop, so
                        // drop radius is one of characteristics of beam
                        // it's name short to be short in math formulas

    constexpr static const double EPS = 0.000001;
    constexpr static const double INF = 100000;

public:
    Beam();
    Beam(double A, double B, double C, double lambda, double radius);
    Beam(double radius);

    double getAngle();
    double getWL();

    void setWL(double wl);
    void setDistance(double d);
    void invertDistance();
    double getDistance();

    void normalizeKoeffs(); // Make more familiar form: y=-a*x-c || x=-c
    void calculateAngle();
    void calculateKoeffs(double x1, double y1, double x2, double y2);
    void calculateInputPoint(double *x0, double *y0);
    void calculateOutputPoint(double *x1, double *y1, double x0, double y0);
    void calculateInfinityPoint(double *x2, double *y2, double x1, double y1);

    inline double fx(double x); // y=f(x) from ax+by+c=0
    inline double fy(double y); // x=g(y) from ax+by+c=0

    friend void cross_ll(Beam A, Beam B, double *x, double *y) {
        *x=(A.b*B.c-B.b*A.c)/(A.a*B.b-B.a*A.b);
        *y=(A.c*B.a-B.c*A.a)/(A.a*B.b-B.a*A.b);
    }

    void rotate(Beam A, double psi);
    void reflect(Beam A);
    void snell(Beam Input, double k);

    double refractIn();
    double refractOut();
};

typedef QList<Beam> Beams;           // in usual C++ using List

#endif // BEAM_H
