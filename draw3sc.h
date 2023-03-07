#ifndef DRAW3SC_H
#define DRAW3SC_H

#include <QOpenGLFunctions>

#include "recalc.h"

void draw_cloud(float w, float h, float R)
{
    glColor3ub(100,100,100);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(fx(15),fy(14));
    for(int i = 0; i <= 50; i++ ) {
                 double  a=i/50.0*M_PI*2.0;
                   glVertex2f(cos(a)*w*fr(R)+fx(15),sin(a)*h*fr(R)+fy(14));
               }
    glEnd();
}

void draw_floor(float ypos)
{
    glColor3ub(0,180,0);
    glBegin(GL_QUADS);
    glVertex2f(0,fy(ypos));
    glVertex2f(X,fy(ypos));
    glVertex2f(X,0);
    glVertex2f(0,0);
    glEnd();
}

void draw_man(float xpos, float ypos, float h)
{
    glColor3ub(255,255,255);
    ///HEAD
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(fx(xpos),fy(ypos+h));
    for(int i = 0; i <= 50; i++ ) {
                 double  a=i/50.0*M_PI*2.0;
                   glVertex2f(cos(a)*fr(0.4)+fx(xpos),sin(a)*fr(0.4)+fy(ypos+h));
               }
    glEnd();

    glBegin(GL_QUADS);
    ///BODY
    glVertex2f(fx(xpos-0.4),fy(ypos+h-0.4));
    glVertex2f(fx(xpos+0.4),fy(ypos+h-0.4));
    glVertex2f(fx(xpos+0.4),fy(ypos));
    glVertex2f(fx(xpos-0.4),fy(ypos));

    ///LEFT HAND
    glVertex2f(fx(xpos+0.7),fy(ypos+h-0.4));
    glVertex2f(fx(xpos+0.4),fy(ypos+h-0.4));
    glVertex2f(fx(xpos+0.4),fy(ypos+h-1.7));
    glVertex2f(fx(xpos+0.7),fy(ypos+h-1.7));

    ///RIGHT HAND
    glVertex2f(fx(xpos-0.72),fy(ypos+h-0.4));
    glVertex2f(fx(xpos-0.4),fy(ypos+h-0.4));
    glVertex2f(fx(xpos-0.4),fy(ypos+h-1.7));
    glVertex2f(fx(xpos-0.72),fy(ypos+h-1.7));
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_POINTS);
    ///EYE
    /*
    glVertex2f(fx(xpos)+5,fy(ypos+h)+4);
//   glVertex2f(fx(xpos)+5,fy(ypos+h)+3);
    glVertex2f(fx(xpos)+4,fy(ypos+h)+3);
    glVertex2f(fx(xpos)+4,fy(ypos+h)+4);
    glVertex2f(fx(xpos)+6,fy(ypos+h)+4);
    glVertex2f(fx(xpos)+6,fy(ypos+h)+3);
    glVertex2f(fx(xpos)+6,fy(ypos+h)+2);
    glVertex2f(fx(xpos)+5,fy(ypos+h)+2);
    glVertex2f(fx(xpos)+4,fy(ypos+h)+2);
    */

    ///LEFT SHOULDER
    glVertex2f(fx(xpos-0.72),fy(ypos+h-0.4));
    glVertex2f(fx(xpos-0.72)+1,fy(ypos+h-0.4));
    glVertex2f(fx(xpos-0.72)+2,fy(ypos+h-0.4));
    glVertex2f(fx(xpos-0.72),fy(ypos+h-0.4)-1);
    glVertex2f(fx(xpos-0.72)+1,fy(ypos+h-0.4)-1);
    glVertex2f(fx(xpos-0.72),fy(ypos+h-0.4)-2);
    ///RIGHT SHOULDER
    glVertex2f(fx(xpos+0.7),fy(ypos+h-0.4));
    glVertex2f(fx(xpos+0.7)-1,fy(ypos+h-0.4));
    glVertex2f(fx(xpos+0.7)-2,fy(ypos+h-0.4));
    glVertex2f(fx(xpos+0.7),fy(ypos+h-0.4)-1);
    glVertex2f(fx(xpos+0.7)-1,fy(ypos+h-0.4)-1);
    glVertex2f(fx(xpos+0.7),fy(ypos+h-0.4)-2);
    ///LEFT HAND
    glVertex2f(fx(xpos-0.72),fy(ypos+h-1.7));
    glVertex2f(fx(xpos-0.4)-1,fy(ypos+h-1.7));
    glVertex2f(fx(xpos-0.72)+1,fy(ypos+h-1.7));
    glVertex2f(fx(xpos-0.4)-2,fy(ypos+h-1.7));
    glVertex2f(fx(xpos-0.72),fy(ypos+h-1.7)+1);
    glVertex2f(fx(xpos-0.4)-1,fy(ypos+h-1.7)+1);
    ///RIGHT HAND
    glVertex2f(fx(xpos+0.7),fy(ypos+h-1.7));
    glVertex2f(fx(xpos+0.4),fy(ypos+h-1.7));
    glVertex2f(fx(xpos+0.7)-1,fy(ypos+h-1.7));
    glVertex2f(fx(xpos+0.4)+1,fy(ypos+h-1.7));
    glVertex2f(fx(xpos+0.7),fy(ypos+h-1.7)+1);
    glVertex2f(fx(xpos+0.4),fy(ypos+h-1.7)+1);
    ///LEFT LEG
    glVertex2f(fx(xpos-0.4),fy(ypos));
    glVertex2f(fx(xpos)-1,fy(ypos));
    glVertex2f(fx(xpos-0.4)+1,fy(ypos));
    glVertex2f(fx(xpos)-2,fy(ypos));
    glVertex2f(fx(xpos-0.4),fy(ypos)+1);
    glVertex2f(fx(xpos)-1,fy(ypos)+1);
    ///RIGHT LEG
    glVertex2f(fx(xpos+0.4)-1,fy(ypos));
    glVertex2f(fx(xpos)+1,fy(ypos));
    glVertex2f(fx(xpos+0.4)-2,fy(ypos));
    glVertex2f(fx(xpos)+2,fy(ypos));
    glVertex2f(fx(xpos+0.4)-1,fy(ypos)+1);
    glVertex2f(fx(xpos)+1,fy(ypos)+1);
    glEnd();
    ///SOME LINES
    glBegin(GL_LINES);
    glVertex2f(fx(xpos),fy(ypos));
    glVertex2f(fx(xpos),fy(ypos+1.33));
    glVertex2f(fx(xpos+0.4),fy(ypos+2.3));
    glVertex2f(fx(xpos+0.4),fy(ypos+1.33));
    glVertex2f(fx(xpos-0.42),fy(ypos+2.3));
    glVertex2f(fx(xpos-0.42),fy(ypos+1.33));
    glEnd();
}

#endif // DRAW3SC_H
