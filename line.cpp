#include "line.h"

Line::Line () {
    a=b=c=0;
    fi=0;
    lambda=0;
    wh=0;
}

Line::Line (double x, double y, double z, double l) {
    a=x;
    b=y;
    c=z;
    wh=0;
    lambda=l;
    devide();
    get_angle();
}

double Line::xi() {
    return fi;
}
double Line::l() {
    return lambda;
}
double Line::w() {
    return wh;
}

void Line::invertz()
{ c=-c; }

void Line::white(bool choice)
{
wh=choice;
}

void Line::setwl(double wl)
{lambda=wl;}

void Line::setd(double d)
{c=d;}

void Line::devide () {
    if (b!=0) {
        a=a/b;
        b=1;
        c=c/b;
    } else if (a!=0) {
        a=1;
        b=0;
        c=c/a;
    } else {
        // impossible case
    }
}

void Line::get_angle () {
    if (a==0 && b==0) {
        // impossible case
    }
    if (a==0)
        fi=0;
    else if (b==0)
        fi=90;
    else {
        fi=(atan(-a)*180)/M_PI;
        if (fi<0) fi+=180;
        if (fi>180) fi-=180;
    }
}

void Line::get_koefs (double x1, double y1, double x2, double y2) {
    a=y1-y2;
    b=x2-x1;
    c=y2*x1-y1*x2;
    devide();
    get_angle();
}

inline double Line::fl(double x) {
    return -a*x-c;
}

inline double Line::fa(double y) {
    return (b*y+c)/(-a);
}

void Line::cross_pp (Line B, double x, double y) { // not using, but extremely useful (seems that correct)
    if (B.b==0) {
        a=0;
        b=1;
        c=-y;
    } else if (B.a!=0) {
        a=-1/B.a;
        b=1;
        c=-(a*x+y);
    }
    get_angle();
}

void Line::rotate (Line A, double psi) {
    /// ROTATE LINE ROUND LINE A
    double x,y;
    cross_ll(*this, A, &x, &y);
    fi+=psi;
    if (fi==90) {
        a=1;
        b=0;
        c=x;
    } else {
        double k=tan((fi*M_PI)/180);
        double z=y-k*x;
        a=-k;
        b=1;
        c=-z;
    }
    if (fi<0) fi+=180;
    if (fi>180) fi-=180;
}

void Line::reflect (Line A) {
    double psi=A.fi-fi;
    this->rotate(A,2*psi);
}

void Line::snell (Line Input, double k) {
    double psi=fi-Input.fi;
    if (psi>90) psi-=180;
    if (psi<-90) psi+=180;
    double betta=(asin(sin((psi*M_PI)/180)/k)*180)/M_PI;
    this->rotate(Input,-betta);
    this->lambda=Input.lambda;
}

void Line::getpoint0(double *x0, double *y0) {
    double D=a*a*c*c-(1+a*a)*(c*c-R*R);
    double x1 = (-(a*c)+sqrt(D))/(1+a*a);
    double x2 = (-(a*c)-sqrt(D))/(1+a*a);

    if (x1<x2) *x0=x1;
    else *x0=x2;
    *y0=fl(*x0);
}

void Line::getpoint1(double *x1, double *y1, double x0, double y0) {
    double D=a*a*c*c-(1+a*a)*(c*c-R*R);
    double p1 = (-(a*c)+sqrt(D))/(1+a*a); //x1
    double p2 = (-(a*c)-sqrt(D))/(1+a*a); //x2

    if (std::abs(p1-x0)>0) *x1=p1;
    else *x1=p2;
    if (std::abs(*x1-x0)<0.00001) {
        D=b*b*c*c-(b*b+a*a)*(c*c-a*a*R*R);
        p1 = (-(b*c)+sqrt(D))/(b*b+a*a); //y1
        p2 = (-(b*c)-sqrt(D))/(b*b+a*a); //y2

        if (std::abs(p1-y0)>0) *y1=p1;
        else *y1=p2;
        *x1=fa(*y1);
    } else *y1=fl(*x1);
}

void Line::getpoint2(double *x2, double *y2, double x1, double y1) {
    double x0,y0;
    getpoint1(&x0, &y0, x1, y1);
    if (std::abs(x1-x0)<0.00001) {
        *y2=(y1-y0)*500;
        *x2=fa(*y2);
    } else {
        *x2=(x1-x0)*500;
        *y2=fl(*x2);
    }
}
