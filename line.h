#ifndef LINE_H
#define LINE_H

#include <cmath>

extern double R;

class Line
{
    double a,b,c, // y=-a*x-c || x=-c
           fi,lambda;
          bool wh;
public:

    Line ();
    Line (double x, double y, double z, double l);

    double xi();
    double l();
    double w();
    void invertz();
    void white(bool choice);
    void setwl(double wl);
    void setd(double d);
    void devide ();
    void get_angle ();
    void get_koefs (double x1, double y1, double x2, double y2);
    inline double fl(double x);
    inline double fa(double y);
    friend void cross_ll (Line A, Line B, double *x, double *y) {
        *x=(A.b*B.c-B.b*A.c)/(A.a*B.b-B.a*A.b);
        *y=(A.c*B.a-B.c*A.a)/(A.a*B.b-B.a*A.b);
    }
    void cross_pp (Line B, double x, double y);
    void rotate (Line A, double psi);
    void reflect (Line A);
    void snell (Line Input, double k);
    void getpoint0(double *x0, double *y0);
    void getpoint1(double *x1, double *y1, double x0, double y0);
    void getpoint2(double *x2, double *y2, double x1, double y1);
};

#endif // LINE_H
