#include <QOpenGLFunctions>

#include <iostream>

#include "beam.h"
#include "recalc.h"
#include "questions.h"
#include "draw3sc.h"
#include "wavelength.h"

extern int
m3beams, // number of beams to count (but not to draw)
radio_ch, // choice of RADIO BUTTON
dynamic; // dynamic image or not

extern float *rnd; // array for random values in 3rd scene

void draw_mode3()
{
    int r,g,b;
    float w=3,h=1,          // koeffs for the ellipse w-width, h-height
          xpos=-5,ypos=-11, // position of MAN
          eyex,eyey,        // position of eye - counts in draw_man(...)
          man_height=3;       // simply man's height
    Beam Ln;
    Ln.calculateKoeffs(-1,0,-0.4,10); // - edge of rain \\\ DON'T TOUCH Y - vars!!!

    /// DRAW RAIN
    glColor3ub(200,200,200);
    glEnable(GL_LINE_SMOOTH); // begin of antialiasing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // end of antialiasing
    glEnable(GL_LINE_STIPPLE); // dashed line
    glLineStipple(10, 0xAAAA); // style of dashes
    glBegin(GL_LINES);
    double xbeg = fx(5),
           xend = fx(12);
    for (double xcur=xbeg; xcur<=xend; xcur+=10)
    {
        glVertex2f(xcur,fye(6+(rand()%500)/100));
        glVertex2f(xcur-50,0);
    }
    // Sleep(0.05);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_STIPPLE);

    draw_cloud(w,h,6);
    draw_floor(ypos);
    eyex=fx(xpos)+6;            // count here because
    eyey=fy(ypos+man_height)+4; // we want to know where is our eye

    int j=0;
    for (double i=-4; i<5; i+=10.0/m3beams)
    {
        Beam Lc(0,1,i,0),Lm;
        double gx,gy,rd=40,   // rd - mini radius [pixels]
                     xcut,ycut; // cutted beam
        cross_ll(Ln,Lc,&gx,&gy);

        if (dynamic)
            for (int i=0; i<m3beams; i++)
                rnd[i]=(float)(rand()%500)/100;

        gx+=rnd[j++];
        Lm.calculateKoeffs(fxe(gx),fye(gy),eyex,eyey);
        /// ORIGINAL BEAMS
        if (j%3==1) // draw every 3rd original beam
        {
            glBegin(GL_LINES);
            glColor3ub(255,255,255);
            glVertex2f(0,fye(gy));
            glVertex2f(fxe(gx),fye(gy));
            glEnd();
        }
        // End of original beams
        if(radio_ch==0)
        {
            for (int w=380; w<=780; w+=60)
            {
                double x=gx-30;
                double y1=gy+tan(what_angle(w,1)*M_PI/180)*(-30);
                double y2=gy+tan(what_angle(w,2)*M_PI/180)*(-30);

                wavelengthToRGB(w,&r,&g,&b);
                glColor3ub(r,g,b);
                glBegin(GL_LINES);
                glVertex2f(fxe(gx),fye(gy));
                glVertex2f(fxe(x),fye(y1));
                glVertex2f(fxe(gx),fye(gy));
                glVertex2f(fxe(x),fye(y2));
                glEnd();
            }
            draw_floor(ypos);
        }
        if (radio_ch==1)
            if ((Lm.getAngle()>=what_angle(380,1) && Lm.getAngle()<=what_angle(780,1)) || (Lm.getAngle()>=what_angle(780,2) && Lm.getAngle()<=what_angle(380,2)))
            {
                xcut=eyex+rd*cos(Lm.getAngle()*M_PI/180);
                ycut=eyey+rd*sin(Lm.getAngle()*M_PI/180);
                if (Lm.getAngle()<=what_angle(780,1))
                    wavelengthToRGB(what_wave(Lm.getAngle(),1),&r,&g,&b);
                else wavelengthToRGB(what_wave(Lm.getAngle(),2),&r,&g,&b);
                glBegin(GL_LINES);
                glColor3ub(255,255,255);
                glColor3ub(r,g,b);
                glVertex2f(fxe(gx),fye(gy));
                glVertex2f(xcut,ycut);
                glEnd();
                draw_floor(ypos);
                draw_man(xpos,ypos,man_height);
            }
        if (radio_ch==2)
            if ((Lm.getAngle()>=what_angle(380,1) && Lm.getAngle()<=what_angle(780,1)) || (Lm.getAngle()>=what_angle(780,2) && Lm.getAngle()<=what_angle(380,2)))
            {
                for (int w=380; w<=780; w+=30)
                {
                    double x=gx-30;
                    double y1=gy+tan(what_angle(w,1)*M_PI/180)*(-30);
                    double y2=gy+tan(what_angle(w,2)*M_PI/180)*(-30);

                    wavelengthToRGB(w,&r,&g,&b);
                    glColor3ub(r,g,b);
                    glBegin(GL_LINES);
                    glVertex2f(fxe(gx),fye(gy));
                    glVertex2f(fxe(x),fye(y1));
                    glVertex2f(fxe(gx),fye(gy));
                    glVertex2f(fxe(x),fye(y2));
                    glEnd();
                }
                draw_floor(ypos);
                draw_man(xpos,ypos,man_height);
            }
    }
}
