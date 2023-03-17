#include "beam.h"

Beam::Beam()
{
    a=b=c=0;
    phi=0;
    wavelength=0;
}

Beam::Beam(double A, double B, double C, double lambda)
{
    a=A;
    b=B;
    c=C;
    wavelength=lambda;
    normalizeKoeffs();
    calculateAngle();
}


double Beam::getAngle()
{
    return phi;
}

double Beam::getWL()
{
    return wavelength;
}

void Beam::setWL(double wl)
{
    wavelength=wl;
}

void Beam::setd(double d)
{
    c=d;
}

void Beam::normalizeKoeffs()
{
    if (b!=0) {
        a=a/b;
        b=1;
        c=c/b;
    } else if (a!=0) {
        a=1;
        b=0;
        c=c/a;
    } else {
        // (a == 0) && (b == 0) is impossible case
    }
}

void Beam::calculateAngle () {
    if ( a==0 )
        phi = 0;
    else if (b == 0)
        phi = 90;
    else {
        phi = (atan(-a) * 180)/M_PI;
        if (phi < 0)
            phi += 180;
        if (phi > 180)
            phi -= 180;
    }
}

void Beam::calculateKoeffs (double x1, double y1, double x2, double y2) {
    a = y1-y2;
    b = x2-x1;
    c = y2*x1-y1*x2;

    normalizeKoeffs();
    calculateAngle();
}

inline double Beam::fx(double x) {
    return -a*x-c;
}

inline double Beam::fy(double y) {
    return (b*y+c)/(-a);
}

void Beam::rotate (Beam A, double psi) {
    /// ROTATE LINE ROUND LINE A
    double x,y;
    cross_ll(*this, A, &x, &y);
    phi+=psi;
    if (phi==90) {
        a=1;
        b=0;
        c=x;
    } else {
        double k=tan((phi*M_PI)/180);
        double z=y-k*x;
        a=-k;
        b=1;
        c=-z;
    }
    if (phi<0) phi+=180;
    if (phi>180) phi-=180;
}

void Beam::reflect (Beam A) {
    double psi = A.phi - phi;
    this->rotate(A,2*psi);
}

void Beam::snell (Beam Input, double k) {
    double psi=phi-Input.phi;
    if (psi>90) psi-=180;
    if (psi<-90) psi+=180;
    double betta=(asin(sin((psi*M_PI)/180)/k)*180)/M_PI;
    this->rotate(Input,-betta);
    this->wavelength=Input.wavelength;
}

void Beam::calculateInputPoint(double *x0, double *y0) {
    double D = a*a*c*c - (1+a*a)*(c*c-R*R);
    double p1 = (-(a*c)+sqrt(D))/(1+a*a);
    double p2 = (-(a*c)-sqrt(D))/(1+a*a);

    if (p1<p2)
        *x0=p1;
    else
        *x0=p2;

    *y0=fx(*x0);
}

void Beam::calculateOutputPoint(double *x1, double *y1, double x0, double y0) {
    double D = a*a*c*c - (1+a*a)*(c*c-R*R);
    double p1 = (-(a*c)+sqrt(D))/(1+a*a);
    double p2 = (-(a*c)-sqrt(D))/(1+a*a);

    if (std::abs(p1-x0) < Beam::EPS)
        *x1=p2;
    else
        *x1=p1;

    if (std::abs(*x1-x0) < Beam::EPS) {
        D = b*b*c*c - (b*b+a*a)*(c*c-a*a*R*R);
        p1 = (-(b*c)+sqrt(D))/(b*b+a*a); //y1
        p2 = (-(b*c)-sqrt(D))/(b*b+a*a); //y2

        if (std::abs(p1-y0) < Beam::EPS)
            *y1=p2;
        else
            *y1=p1;

        *x1=fy(*y1);
    } else
        *y1=fx(*x1);
}

void Beam::calculateInfinityPoint(double *x2, double *y2, double x1, double y1) {
    double x0, y0; // temporary calculation of output for correct direction
    calculateOutputPoint(&x0, &y0, x1, y1);

    if (std::abs(x1-x0) < Beam::EPS) {
        *y2 = (y1-y0) * Beam::INF;
        *x2 = fy(*y2);
    } else { // General case
        *x2 = (x1-x0) * Beam::INF;
        *y2 = fx(*x2);
    }
}
