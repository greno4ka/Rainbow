#ifndef BEAM_H
#define BEAM_H

#include <QList>

#include <cmath>

extern double R;

class Beam
{
    double a,b,c,       // koefficients in equation ax+y+c=0
    phi,                // positive angle of beam line
                        // it's physical simulation, so we need angle
                        // only in physical sense, not in mathematical
    wavelength;

    constexpr static const double EPS = 0.0000000001;
    constexpr static const double INF = 10000000000;

public:
    Beam ();
    Beam (double A, double B, double C, double lambda);

    double getAngle();
    double getWL();

    void setWL(double wl);
    void setd(double d);

    void normalizeKoeffs(); // Make more familiar form: y=-a*x-c || x=-c
    void calculateAngle();
    void calculateKoeffs(double x1, double y1, double x2, double y2);

    inline double fx(double x); // y=f(x) from ax+by+c=0
    inline double fy(double y); // x=g(y) from ax+by+c=0

    friend void cross_ll (Beam A, Beam B, double *x, double *y) {
        *x=(A.b*B.c-B.b*A.c)/(A.a*B.b-B.a*A.b);
        *y=(A.c*B.a-B.c*A.a)/(A.a*B.b-B.a*A.b);
    }

    void rotate (Beam A, double psi);
    void reflect (Beam A);
    void snell (Beam Input, double k);
    void calculateInputPoint(double *x0, double *y0);
    void calculateOutputPoint(double *x1, double *y1, double x0, double y0);
    void calculateInfinityPoint(double *x2, double *y2, double x1, double y1);

    void invertz()
    { c=-c; }
};

typedef QList<Beam> Beams;           // in usual C++ using List

#endif // BEAM_H
