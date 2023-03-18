#include "scene3.h"

#include "wavelength.h"

Scene3::Scene3()
{
    displayMode = 0;
    // double Z=std::min(X/4,Y/2);
}

void Scene3::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}

void Scene3::switchDynamicMode()
{
    dynamicMode = !dynamicMode;
}

inline double Scene3::x(double x0)
{
    return X-(X/8)+x0*std::min(X/8,Y/4)/Scale;
}

inline double Scene3::y(double y0)
{
    return (Y/4)+y0*std::min(X/8,Y/4)/Scale;
}

inline double Scene3::r(double r0)
{
    return r0*std::min(X/8,Y/4)/Scale;
}

void Scene3::display()
{
    int r,g,b;
    float w=3,h=1,          // koeffs for the ellipse w-width, h-height
          xpos=-5,ypos=-11, // position of MAN
          eyex,eyey,        // position of eye - counts in draw_man(...)
          man_height=3;       // simply man's height
    float *rnd; // array for random values in 3rd scene
    int m3beams = 100;
    Beam Ln(1);
    Ln.calculateKoeffs(-1,0,-0.4,10); // - edge of rain \\\ DON'T TOUCH Y - vars!!!
    rnd = (float*)malloc((m3beams) * sizeof(float));
    /// DRAW RAIN
    glColor3ub(200,200,200);
    glEnable(GL_LINE_SMOOTH); // begin of antialiasing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // end of antialiasing
    glEnable(GL_LINE_STIPPLE); // dashed line
    glLineStipple(10, 0xAAAA); // style of dashes
    glBegin(GL_LINES);
    double xbeg = x(5),
           xend = x(12);
    for (double xcur=xbeg; xcur<=xend; xcur+=10)
    {
        glVertex2f(xcur,y(6+(rand()%500)/100));
        glVertex2f(xcur-50,0);
    }
    // Sleep(0.05);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_STIPPLE);

    draw_cloud(w,h,6);
    draw_floor(ypos);
    eyex=x(xpos)+6;            // count here because
    eyey=y(ypos+man_height)+4; // we want to know where is our eye

    int j=0;
    for (double i=-4; i<5; i+=10.0/m3beams)
    {
        Beam Lc(0,1,i,0,1),Lm(1);
        double gx,gy,rd=40,   // rd - mini radius [pixels]
                     xcut,ycut; // cutted beam
        cross_ll(Ln,Lc,&gx,&gy);

        if (dynamicMode)
            for (int i=0; i<m3beams; i++)
                rnd[i]=(float)(rand()%500)/100;

        gx+=rnd[j++];
        Lm.calculateKoeffs(x(gx),y(gy),eyex,eyey);
        /// ORIGINAL BEAMS
        if (j%3==1) // draw every 3rd original beam
        {
            glBegin(GL_LINES);
            glColor3ub(255,255,255);
            glVertex2f(0,y(gy));
            glVertex2f(x(gx),y(gy));
            glEnd();
        }

        // End of original beams
        if(displayMode==0)
        {
            for (int w=380; w<=780; w+=60)
            {
                double x0=gx-30;
                double y1=gy+tan(whatAngle(w,1)*M_PI/180)*(-30);
                double y2=gy+tan(whatAngle(w,2)*M_PI/180)*(-30);

                wavelengthToRGB(w,&r,&g,&b);
                glColor3ub(r,g,b);
                glBegin(GL_LINES);
                glVertex2f(x(gx),y(gy));
                glVertex2f(x(x0),y(y1));
                glVertex2f(x(gx),y(gy));
                glVertex2f(x(x0),y(y2));
                glEnd();
            }
            draw_floor(ypos);
        }
        if (displayMode == 1)
            if ((Lm.getAngle()>=whatAngle(380,1) && Lm.getAngle()<=whatAngle(780,1)) || (Lm.getAngle()>=whatAngle(780,2) && Lm.getAngle()<=whatAngle(380,2)))
            {
                xcut=eyex+rd*cos(Lm.getAngle()*M_PI/180);
                ycut=eyey+rd*sin(Lm.getAngle()*M_PI/180);
                if (Lm.getAngle()<=whatAngle(780,1))
                    wavelengthToRGB(whatWave(Lm.getAngle(),1),&r,&g,&b);
                else wavelengthToRGB(whatWave(Lm.getAngle(),2),&r,&g,&b);
                glBegin(GL_LINES);
                glColor3ub(255,255,255);
                glColor3ub(r,g,b);
                glVertex2f(x(gx),y(gy));
                glVertex2f(xcut,ycut);
                glEnd();
                draw_floor(ypos);
                draw_man(xpos,ypos,man_height);
            }
        if (displayMode == 2)
            if ((Lm.getAngle()>=whatAngle(380,1) && Lm.getAngle()<=whatAngle(780,1)) || (Lm.getAngle()>=whatAngle(780,2) && Lm.getAngle()<=whatAngle(380,2)))
            {
                for (int w=380; w<=780; w+=30)
                {
                    double x0=gx-30;
                    double y1=gy+tan(whatAngle(w,1)*M_PI/180)*(-30);
                    double y2=gy+tan(whatAngle(w,2)*M_PI/180)*(-30);

                    wavelengthToRGB(w,&r,&g,&b);
                    glColor3ub(r,g,b);
                    glBegin(GL_LINES);
                    glVertex2f(x(gx),y(gy));
                    glVertex2f(x(x0),y(y1));
                    glVertex2f(x(gx),y(gy));
                    glVertex2f(x(x0),y(y2));
                    glEnd();
                }
                draw_floor(ypos);
                draw_man(xpos,ypos,man_height);
            }
    }
}

void Scene3::draw_cloud(float w, float h, float R)
{
    glColor3ub(100,100,100);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(15),y(14));
    for(int i = 0; i <= 50; i++ ) {
                 double  a=i/50.0*M_PI*2.0;
                   glVertex2f(cos(a)*w*r(R)+x(15),sin(a)*h*r(R)+y(14));
               }
    glEnd();
}

void Scene3::draw_floor(float ypos)
{
    glColor3ub(0,180,0);
    glBegin(GL_QUADS);
    glVertex2f(0,y(ypos));
    glVertex2f(X,y(ypos));
    glVertex2f(X,0);
    glVertex2f(0,0);
    glEnd();
}

void Scene3::draw_man(float xpos, float ypos, float h)
{
    glColor3ub(255,255,255);
    ///HEAD
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(xpos),y(ypos+h));
    for(int i = 0; i <= 50; i++ ) {
                 double  a=i/50.0*M_PI*2.0;
                   glVertex2f(cos(a)*r(0.4)+x(xpos),sin(a)*r(0.4)+y(ypos+h));
               }
    glEnd();

    glBegin(GL_QUADS);
    ///BODY
    glVertex2f(x(xpos-0.4),y(ypos+h-0.4));
    glVertex2f(x(xpos+0.4),y(ypos+h-0.4));
    glVertex2f(x(xpos+0.4),y(ypos));
    glVertex2f(x(xpos-0.4),y(ypos));

    ///LEFT HAND
    glVertex2f(x(xpos+0.7),y(ypos+h-0.4));
    glVertex2f(x(xpos+0.4),y(ypos+h-0.4));
    glVertex2f(x(xpos+0.4),y(ypos+h-1.7));
    glVertex2f(x(xpos+0.7),y(ypos+h-1.7));

    ///RIGHT HAND
    glVertex2f(x(xpos-0.72),y(ypos+h-0.4));
    glVertex2f(x(xpos-0.4),y(ypos+h-0.4));
    glVertex2f(x(xpos-0.4),y(ypos+h-1.7));
    glVertex2f(x(xpos-0.72),y(ypos+h-1.7));
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_POINTS);
    ///LEFT SHOULDER
    glVertex2f(x(xpos-0.72),y(ypos+h-0.4));
    glVertex2f(x(xpos-0.72)+1,y(ypos+h-0.4));
    glVertex2f(x(xpos-0.72)+2,y(ypos+h-0.4));
    glVertex2f(x(xpos-0.72),y(ypos+h-0.4)-1);
    glVertex2f(x(xpos-0.72)+1,y(ypos+h-0.4)-1);
    glVertex2f(x(xpos-0.72),y(ypos+h-0.4)-2);
    ///RIGHT SHOULDER
    glVertex2f(x(xpos+0.7),y(ypos+h-0.4));
    glVertex2f(x(xpos+0.7)-1,y(ypos+h-0.4));
    glVertex2f(x(xpos+0.7)-2,y(ypos+h-0.4));
    glVertex2f(x(xpos+0.7),y(ypos+h-0.4)-1);
    glVertex2f(x(xpos+0.7)-1,y(ypos+h-0.4)-1);
    glVertex2f(x(xpos+0.7),y(ypos+h-0.4)-2);
    ///LEFT HAND
    glVertex2f(x(xpos-0.72),y(ypos+h-1.7));
    glVertex2f(x(xpos-0.4)-1,y(ypos+h-1.7));
    glVertex2f(x(xpos-0.72)+1,y(ypos+h-1.7));
    glVertex2f(x(xpos-0.4)-2,y(ypos+h-1.7));
    glVertex2f(x(xpos-0.72),y(ypos+h-1.7)+1);
    glVertex2f(x(xpos-0.4)-1,y(ypos+h-1.7)+1);
    ///RIGHT HAND
    glVertex2f(x(xpos+0.7),y(ypos+h-1.7));
    glVertex2f(x(xpos+0.4),y(ypos+h-1.7));
    glVertex2f(x(xpos+0.7)-1,y(ypos+h-1.7));
    glVertex2f(x(xpos+0.4)+1,y(ypos+h-1.7));
    glVertex2f(x(xpos+0.7),y(ypos+h-1.7)+1);
    glVertex2f(x(xpos+0.4),y(ypos+h-1.7)+1);
    ///LEFT LEG
    glVertex2f(x(xpos-0.4),y(ypos));
    glVertex2f(x(xpos)-1,y(ypos));
    glVertex2f(x(xpos-0.4)+1,y(ypos));
    glVertex2f(x(xpos)-2,y(ypos));
    glVertex2f(x(xpos-0.4),y(ypos)+1);
    glVertex2f(x(xpos)-1,y(ypos)+1);
    ///RIGHT LEG
    glVertex2f(x(xpos+0.4)-1,y(ypos));
    glVertex2f(x(xpos)+1,y(ypos));
    glVertex2f(x(xpos+0.4)-2,y(ypos));
    glVertex2f(x(xpos)+2,y(ypos));
    glVertex2f(x(xpos+0.4)-1,y(ypos)+1);
    glVertex2f(x(xpos)+1,y(ypos)+1);
    glEnd();
    ///SOME LINES
    glBegin(GL_LINES);
    glVertex2f(x(xpos),y(ypos));
    glVertex2f(x(xpos),y(ypos+1.33));
    glVertex2f(x(xpos+0.4),y(ypos+2.3));
    glVertex2f(x(xpos+0.4),y(ypos+1.33));
    glVertex2f(x(xpos-0.42),y(ypos+2.3));
    glVertex2f(x(xpos-0.42),y(ypos+1.33));
    glEnd();
}
