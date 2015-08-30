#ifndef DRAWBEAM_H
#define DRAWBEAM_H

#include <iostream>
#include <GL\glut.h>
#include "wavelength.h"

using namespace std;

extern int mode,
       radio_ch,
       brbeams,
       rainbows,
       angle, N;
extern double wl;
extern bool done;

extern inline double k(double x);
extern double what_angle(double wave, int var);
extern int invert_wave(int wave);

void draw_string_bitmap(void *font, const char* string)
{
// Draw strings, using glut [in 2nd scene draw angle]
    while (*string)
        glutBitmapCharacter(font, *string++);
}
/*
void draw_beam0(Line Input)
{
    double p=1,   // GAMMA CORRECTOR of color. For dark beams
           x0,y0, // point0
           x1,y1, // point1
           x2,y2; // point2 - external (for reformed)
    int r,g,b;    // color of line
    Line Reformed, Rad, Output; // Rad - Radius to the point1 , Output - first-first reflection

    WavelengthToRGB(Input.l(),&r,&g,&b);
    Input.getpoint0(&x0, &y0);




    }
    for (int z=1; z<=N; z++)
    {
        p-=0.2;
        Rad.get_koefs(x1,y1,0,0);

        Reformed=Rad; // we're get reformed from radius again
        Reformed.snell(Input,1/k(Input.l())); // 1/k cause goin' from inside out
        Reformed.getpoint2(&x2,&y2,x1,y1);

        glColor3ub(r*p,g*p,b*p);
        glBegin(GL_LINES);
/// DRAW REFORMED BEAM
       // if(!((z==2) && x2>0)) // x2 - to kill one bug
            if(!rainbows || (radio_ch==1 && z==2) || (radio_ch==2 && z==3))
            {
                    glVertex2f(fx(x1),fy(y1));
                    glVertex2f(fx(x2),fy(y2));
            }
        glEnd();

        Input.reflect(Rad);
        x0=x1;
        y0=y1;
        Input.getpoint1(&x1, &y1, x0, y0);
        /// DRAW EXACT BEAM
        glBegin(GL_LINES);
        glVertex2f(fx(x0),fy(y0));
        glVertex2f(fx(x1),fy(y1));
        glEnd();
    }
}
*/
void draw_beam(Line Input)
{
    double p=1,   // GAMMA CORRECTOR of color. For dark beams
           x0,y0, // point0
           x1,y1, // point1
           x2,y2; // point2 - external (for reformed)
    int r,g,b;    // color of line
    bool flag=false;    // if Input distance belongs to [0.9;0.95]
    Line Reformed, Rad, Output; // Rad - Radius to the point1 , Output - first-first reflection

    WavelengthToRGB(Input.l(),&r,&g,&b);
    glColor3ub(r*p,g*p,b*p);
    // THIS SCENE IS ABSOLUTELY FUCKING STUPID!!!
    // uncomment if want to invert colors
    /// if (mode==4) WavelengthToRGB(invert_wave(Input.l()),&r,&g,&b);

    Input.getpoint0(&x0, &y0);

    if (brbeams)
        if ((((y0>=0 && y0<=0.85*R) || y0>=0.90*R) && radio_ch==0) ||
                (((y0>=0 && y0<=0.94*R) || y0>=0.99*R) && radio_ch==1) ||
                (((y0>=-0.94*R && y0<=0) || y0<=-0.99*R) && radio_ch==1)) flag=true;

    if(mode==0)
    {
/// ORIGINAL BEAM FOR 0 SCENE
        glBegin(GL_LINES);
        glVertex2f(0,fy(y0));
        glVertex2f(fx(x0),fy(y0));
        glEnd();
// End of Original beam
        if(N>=0)
        {
// First reflection code ONLY IN 0 SCENE
            Rad.get_koefs(x0,y0,0,0);
            Output=Input;
            Output.reflect(Rad);
            Output.getpoint2(&x2,&y2,x0,y0);
            if (!rainbows)
            {
                glBegin(GL_LINES);
                glVertex2f(fx(x0),fy(y0));
                glVertex2f(fx(x2),fy(y2));
                glEnd();
            }
// end of first reflection

// First refraction code
            Reformed=Rad; // we're get reformed from radius
            Reformed.snell(Input,k(Input.l()));
            Input=Reformed;

            Input.getpoint1(&x1, &y1, x0, y0);
            glBegin(GL_LINES);
            glVertex2f(fx(x0),fy(y0));
            glVertex2f(fx(x1),fy(y1));
            glEnd();

// end of first refraction
        }
    }
    else
    {
/// ORIGINAL BEAM
        glBegin(GL_LINES);
        if (mode==1 || mode==2)
        {
            if(!flag)
            {
                glColor3ub(r,g,b);
                glVertex2f(0,fye(y0));
                glVertex2f(fxe(x0),fye(y0));
            }
            else
            {
                glColor3ub(r*0.5,g*0.5,b*0.5);
                glVertex2f(0,fye(y0));
                glVertex2f(fxe(x0),fye(y0));
            }
        }
        glEnd();
// end of original beam

    Reformed=Rad; // we're get reformed from radius
    Reformed.snell(Input,k(Input.l()));
    Input=Reformed;
    glLineWidth(1);
    }
    for (int z=1; z<=N; z++)
    {
        if (mode==0) p-=0.2;
        else switch (z)
            {
            case 1:
                p=1;
                break;
            case 2:
                if (mode==4) p=0.85;
                else p=0.9;
                break;
            case 3:
                if (mode==4) p=0.55;
                else p=0.8;
                break;
            }
        Rad.get_koefs(x1,y1,0,0);

        Reformed=Rad; // we're get reformed from radius again
        Reformed.snell(Input,1/k(Input.l())); // 1/k cause goin' from inside out
        Reformed.getpoint2(&x2,&y2,x1,y1);

        if (mode!=0)
        {
            Input.getpoint1(&x1, &y1, x0, y0);
            Rad.get_koefs(x1,y1,0,0);

            Reformed=Rad; // we're get reformed from radius again
            Reformed.snell(Input,1/k(Input.l())); // 1/k cause goin' from inside out
            Reformed.getpoint2(&x2,&y2,x1,y1);

            glColor3ub(r*p,g*p,b*p);
            glBegin(GL_LINES);
/// DRAW EXACT BEAM
            if (mode==1 || mode==2)
            {
                if(!flag)
                {
                    glColor3ub(r*p,g*p,b*p);
                    glVertex2f(fxe(x0),fye(y0));
                    glVertex2f(fxe(x1),fye(y1));
                }
                else
                {
                    glColor3ub(r*0.5,g*0.5,b*0.5);
                    glVertex2f(fxe(x0),fye(y0));
                    glVertex2f(fxe(x1),fye(y1));
                }
            }
            else if (mode==4)
            {
                glVertex2f(fx4(x0),fy4(y0));
                glVertex2f(fx4(x1),fy4(y1));
            }
        }
/// DRAW REFORMED BEAM
        //if(!((z==2) && x2>0)) // x2 - to kill one bug
            if(!rainbows || (z==2 && radio_ch==0) || (z==3 && radio_ch==1))
            {
                if (mode==0)
                {
                    glVertex2f(fx(x1),fy(y1));
                    glVertex2f(fx(x2),fy(y2));
                }
                else if (mode==1 || mode==2)
                {
                    if(!flag)
                    {
                        glColor3ub(r*p,g*p,b*p);
                        glVertex2f(fxe(x1),fye(y1));
                        glVertex2f(fxe(x2),fye(y2));
                    }
                    else
                    {
                        glColor3ub(r*0.5,g*0.5,b*0.5);
                        glVertex2f(fxe(x1),fye(y1));
                        glVertex2f(fxe(x2),fye(y2));
                    }
                }
                else if (mode==4)
                {
                    if ((radio_ch==0 && ((z==2) || (z==1))) || (radio_ch==1 && z==3) || radio_ch==2)
                        glVertex2f(fx4(x1),fy4(y1));
                    glVertex2f(fx4(x2),fy4(y2));
                }
            }
        glEnd();
        if (mode==2)
            if ((z==2 && (Reformed.xi()>=what_angle(wl,1)-0.1 && Reformed.xi()<=what_angle(wl,1)+0.1) && done==false && angle) || (z==3 && Reformed.xi()<=what_angle(wl,2)+0.05 && done==false && angle))
            {
                Line Hor(0,1,14,wl);
                double ox,oy;
                cross_ll(Reformed,Hor,&ox,&oy);
                glLineWidth(3);
                glColor3ub(255,255,255);
                glBegin(GL_LINES);
                if (z==2)
                {
                    glVertex2f(fxe(ox)+4,fye(oy));
                    glVertex2f(fxe(ox+2),fye(oy));
                }
                else
                {
                    glVertex2f(fxe(ox)-2,fye(oy));
                    glVertex2f(fxe(ox+2),fye(oy));
                }
                glEnd();
                glLineWidth(1);
                glColor3ub(255,255,255);
                if (z==2)
                    glRasterPos2f(fxe(ox+2), fye(oy)+5);
                else
                    glRasterPos2f(fxe(ox-2), fye(oy)+5);
                char buffer [10];
                if (z==2)
                    sprintf(buffer, "%2.2f", what_angle(wl,1));
                if (z==3)
                    sprintf(buffer, "%2.2f", what_angle(wl,2));

                draw_string_bitmap(GLUT_BITMAP_TIMES_ROMAN_24 , buffer);
                done=true;
                glBegin(GL_TRIANGLE_FAN);
                double tmp;
                if (z==2) tmp=fxe(ox)+4;
                else tmp=fxe(ox)-2;
                glVertex2f(tmp,fye(oy));
                double a=0.0;
                do
                {
                    glVertex2f(tmp+fre(1)*cos(a),fye(oy)+fre(1)*sin(a));
                    a=a*180/PI;
                    a=a+0.05;
//        cout << a << endl;
                    a=a*PI/180;
                }
                while (a*180/PI<=what_angle(wl,z-1)-0.01);
                glEnd();
            }

        Input.reflect(Rad);
        x0=x1;
        y0=y1;
        if (mode==0)
        {
            Input.getpoint1(&x1, &y1, x0, y0);
            /// DRAW EXACT BEAM
            glBegin(GL_LINES);
            glVertex2f(fx(x0),fy(y0));
            glVertex2f(fx(x1),fy(y1));
            glEnd();
        }
    }
}
#endif // DRAWBEAM_H
