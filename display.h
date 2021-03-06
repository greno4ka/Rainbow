#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <list>
#include <cmath>
#include "recalc.h"
#include "line.h"
#include "draw_beam.h"
#include "questions.h"
#include "draw3sc.h"
#include "mode3.h"

const double PI=3.14159265359;     // We have our own PI
typedef QList<Line> Lst;           // in usual C++ using List
extern Lst Beams;                  // STL list of our beams
extern int mode,                   // get mode from main.cpp
       angle,N,                // angle-flag and N - number of refractions
       radio_ch;               // universal variable for selection something
extern bool
one, // flags for 2nd scene
two; // flags for 2nd scene
bool done;

/// //////////////////////////////////////////////////////////////////////////
/// General scheme of Display in modes 0,1,2,5
/// --------------------------------------------
/// + Draw drop
/// + Draw axes
/// + Draw beams
/// //////////////////////////////////////////////////////////////////////////

void draw_drop()
{
    double a;
    glColor3ub(100,100,255);
    if (mode==4) glColor3ub(150,150,150);
    glBegin(GL_TRIANGLE_FAN);
    if (mode==0)
        glVertex2f(fx(0),fy(0));
    else if (mode==1 || mode==2 || mode==5)
        glVertex2f(fxe(0),fye(0));
    else if (mode==4)
        glVertex2f(fx4(0),fy4(0));
    for(int i=0;i<=50;i++) // this code belongs to Anton Standrik
    {
        a = (float)i/50.0*PI*2.0;
        if (mode==0)
            glVertex2f(fx(0)+fr(R)*cos(a),fy(0)+fr(R)*sin(a));
        else if (mode==1 || mode==2 || mode==5)
            glVertex2f(fxe(0)+fre(R)*cos(a),fye(0)+fre(R)*sin(a));
        else if (mode==4)
            glVertex2f(fx4(0)+fr4(R)*cos(a),fy4(0)+fr4(R)*sin(a));
    }
    glEnd();
}

void Display()
{
    double x0,y0;

    if (mode==3) draw_mode3(); // that's simple, isn't it?
    else
    {
        if (mode==0)
        {
            if (radio_ch==0) rainbows=0;
            else rainbows=1;
            if (radio_ch==1) N=2;
            if (radio_ch==2) N=3;
        }
        else
            if (radio_ch==0) N=2;    // instead steps
            else N=3;
        // here was a bug

/// DROP
        draw_drop();
// End of DROP

/// AXES
        glColor3ub(255,255,255);
        glEnable(GL_LINE_STIPPLE); // turn on - - - - - -
        glLineStipple(1, 0x1111); // 1 , 1111 means tiny dashes
        glBegin(GL_LINES);
        if (mode==0)
        {
            glVertex2f(0,fy(0));
            glVertex2f(X,fy(0));
            glVertex2f(fx(0),0);
            glVertex2f(fx(0),Y);
        }
        else if (mode==1 || mode==2)
        {
            glVertex2f(0,fye(0));
            glVertex2f(X,fye(0));
            glVertex2f(fxe(0),0);
            glVertex2f(fxe(0),Y);
        }
        // no nessecity to draw axes in 3rd and 4th scenes
        glEnd();
        glDisable(GL_LINE_STIPPLE); // turn it off
// End of Axes

        if (mode!=2) // mode can be 0,1 or 4
            for (Lst::iterator i=Beams.begin(); i!=Beams.end(); i++)
            {
                if (mode==0) draw_beam0(*i);
                else draw_beam(*i);
                i->getpoint0(&x0, &y0);
            }

/// //////////////////////////////////////////////////////////////////////////
/// /// This part of code responds for invertion of beams in 2nd scene     ///
/// //////////////////////////////////////////////////////////////////////////
        else // if mode==2
        {
            done=false; // we want to draw number of degree only one time
            bool miniflag1=false,
                 miniflag2=false;
            for (Lst::iterator i=Beams.begin(); i!=Beams.end(); i++)
            {
                if (radio_ch==1 && angle && !one)
                {
                    i->invertz();
                    miniflag1=true;
                    two=true;
                }
                else if (radio_ch==1 && !angle && two)
                {
                    i->invertz();
                    miniflag2=true;
                    one=false;
                }

                if (radio_ch==0 && two==true)
                {
                    i->invertz();
                    one=false;
                    miniflag2=true;
                }

                draw_beam(*i); // standart function
                i->getpoint0(&x0, &y0); // too
            }
            if (miniflag1) one=true;
            if (miniflag2) two=false;
        }
/// //////////////////////////////////////////////////////////////////////////

        if (mode==4) // DRAW AFTER ALL BEAMS
        {
            /// DROP
            draw_drop();
            // End of DROP

            /// ORIGINAL LIGHT STREAM
            glColor3ub(255,255,255);
            glBegin(GL_LINES);
            Lst::iterator i;
            for (i=Beams.begin(); i!=Beams.end(); i++)
            {
                i->getpoint0(&x0, &y0);
                glVertex2f(0,fy4(y0));
                glVertex2f(fx4(x0),fy4(y0));
            }
            glEnd();
            // End of light stream
        }
    }
    glFinish();
    //glutSwapBuffers(); // turn on only with double buffer
}

#endif // DISPLAY_H
