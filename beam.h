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
    int red,green,blue; // color components of beam

    // TODO: remove
    bool wh;

public:
    Beam ();
    Beam (double A, double B, double C, double lambda);

    double getAngle();
    double getWL();

    double w();
    void white(bool choice);
    void setwl(double wl);
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
    void getpoint0(double *x0, double *y0);
    void getpoint1(double *x1, double *y1, double x0, double y0);
    void getpoint2(double *x2, double *y2, double x1, double y1);
    void invertz()
    { c=-c; }
};

typedef QList<Beam> Lst;           // in usual C++ using List

#endif // BEAM_H