#include "scene3.h"

#include "wavelength.h"

Scene3::Scene3()
{
    displayMode = 0;
    desiredFPS = 60;
    // double Z=std::min(X/4,Y/2);
}

void Scene3::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}

void Scene3::setDesiredFPS(int newDesiredFPS)
{
    desiredFPS = newDesiredFPS;
}

void Scene3::switchDynamicMode()
{
    dynamicMode = !dynamicMode;
}

inline double Scene3::x(double x0)
{
    return X-(X/4)+x0*std::min(X/4,Y/2)/Scale;
}

inline double Scene3::y(double y0)
{
    return (Y/2)+y0*std::min(X/4,Y/2)/Scale;
}

inline double Scene3::r(double r0)
{
    return r0*std::min(X/4,Y/2)/Scale;
}

void Scene3::display()
{
    int r,g,b;
    double ManPositionX=-5,ManPositionY=-11, // position of MAN
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
    Sleep(888/desiredFPS);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_STIPPLE);

    drawFloor();
    eyex=x(ManPositionX)+6;            // count here because
    eyey=y(ManPositionY+man_height)+4; // we want to know where is our eye

//    glEnable(GL_LINE_SMOOTH); // begin of antialiasing
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // end of antialiasing

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
            glBegin(GL_LINES);
            glColor3ub(255,255,255);
            glVertex2f(0,y(gy));
            glVertex2f(x(gx),y(gy));
            glEnd();

        // End of original beams
        if (displayMode == 0)
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
            drawFloor();
        }
        if (displayMode == 1)
            if ( (Lm.getAngle() >= whatAngle(380,1) && Lm.getAngle() <= whatAngle(780,1) ) ||
                 (Lm.getAngle() >= whatAngle(780,2) && Lm.getAngle() <= whatAngle(380,2) )
               )
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
                drawMan();
                drawFloor();
            }
        if (displayMode == 2)
            if ( (Lm.getAngle() >= whatAngle(380,1) && Lm.getAngle() <= whatAngle(780,1) ) ||
                 (Lm.getAngle() >= whatAngle(780,2) && Lm.getAngle() <= whatAngle(380,2) )
               )
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
                drawFloor();
                drawMan();
            }
    }

    drawCloud();

//    glDisable(GL_LINE_SMOOTH);
//    glDisable(GL_BLEND);
}

void Scene3::drawCloud()
{
    const double CloudWidth = 3,
                 CloudHeight = 1,
                 CloudRadius = 6,
                 CloudCenterX = 15,
                 CloudCenterY = 15;

    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);

    glColor3ub(100,100,100);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(CloudCenterX),y(CloudCenterY));
    for (int i = 0; i <= ImageQuality; i++) {
        double a = (double)i/ImageQuality*M_PI*2;
        glVertex2f(cos(a) * CloudWidth * r(CloudRadius) + x(CloudCenterX),
                   sin(a) * CloudHeight * r(CloudRadius) + y(CloudCenterY));
    }
    glEnd();

    // Disable antialising
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
}

void Scene3::drawFloor()
{
    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);

    glColor3ub(0,180,0);
    glBegin(GL_QUADS);
    glVertex2f(0,y(ManPositionY));
    glVertex2f(X,y(ManPositionY));
    glVertex2f(X,0);
    glVertex2f(0,0);
    glEnd();

    // Disable antialising
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
}

void Scene3::drawMan()
{
    const double ManHeadRadius = 0.4,
                 ManArmWidth = 0.3,
                 ManArmHeight = 1.2;

    glColor3ub(255,255,255);

    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);

    ///HEAD
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(ManPositionX),y(ManPositionY+ManHeight));
    for (int i = 0; i <= ImageQuality; i++ ) {
        double  a = (double)i/ImageQuality*M_PI*2;
        glVertex2f(cos(a)*r(ManHeadRadius)+x(ManPositionX),
                   sin(a)*r(ManHeadRadius)+y(ManPositionY+ManHeight));
    }
    glEnd();

    // Disable antialising
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);

    glBegin(GL_QUADS);
    // points go in clockwise order
    // 1 2
    // 4 3
    /// BODY
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY));
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY));

    /// LEFT HAND
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));

    /// RIGHT HAND
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glEnd();

    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);

    glColor3ub(0,0,0);
    glBegin(GL_POINTS);
    /// LEFT SHOULDER
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+1,y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+2,y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+3,y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius)-1);
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+1,y(ManPositionY+ManHeight-ManHeadRadius)-1);
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius)-2);
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius)-3);

    /// RIGHT SHOULDER
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-1,y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-2,y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-3,y(ManPositionY+ManHeight-ManHeadRadius));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius)-1);
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-1,y(ManPositionY+ManHeight-ManHeadRadius)-1);
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius)-2);
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius)-3);

    /// LEFT HAND
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+2,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+3,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth)+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+2);
    glVertex2f(x(ManPositionX-ManHeadRadius-ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+3);

    glVertex2f(x(ManPositionX-ManHeadRadius)-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius)-1-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius)-1-2,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius)-1-3,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius)-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX-ManHeadRadius)-1-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX-ManHeadRadius)-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+2);
    glVertex2f(x(ManPositionX-ManHeadRadius)-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+3);

    /// RIGHT HAND
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-2,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-3,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth)-1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+2);
    glVertex2f(x(ManPositionX+ManHeadRadius+ManArmWidth),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+3);

    glVertex2f(x(ManPositionX+ManHeadRadius)+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius)+1+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius)+1+2,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius)+1+3,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX+ManHeadRadius)+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX+ManHeadRadius)+1+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+1);
    glVertex2f(x(ManPositionX+ManHeadRadius)+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+2);
    glVertex2f(x(ManPositionX+ManHeadRadius)+1,y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight)+3);

    /// LEFT LEG
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY));
    glVertex2f(x(ManPositionX-ManHeadRadius)+1,y(ManPositionY));
    glVertex2f(x(ManPositionX-ManHeadRadius)+2,y(ManPositionY));
    glVertex2f(x(ManPositionX-ManHeadRadius)+3,y(ManPositionY));
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY)+1);
    glVertex2f(x(ManPositionX-ManHeadRadius)+1,y(ManPositionY)+1);
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY)+2);
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY)+3);

    glVertex2f(x(ManPositionX)-1,y(ManPositionY));
    glVertex2f(x(ManPositionX)-1-1,y(ManPositionY));
    glVertex2f(x(ManPositionX)-1-2,y(ManPositionY));
    glVertex2f(x(ManPositionX)-1-3,y(ManPositionY));
    glVertex2f(x(ManPositionX)-1,y(ManPositionY)+1);
    glVertex2f(x(ManPositionX)-1-1,y(ManPositionY)+1);
    glVertex2f(x(ManPositionX)-1,y(ManPositionY)+2);
    glVertex2f(x(ManPositionX)-1,y(ManPositionY)+3);

    /// RIGHT LEG
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY));
    glVertex2f(x(ManPositionX+ManHeadRadius)-1,y(ManPositionY));
    glVertex2f(x(ManPositionX+ManHeadRadius)-2,y(ManPositionY));
    glVertex2f(x(ManPositionX+ManHeadRadius)-3,y(ManPositionY));
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY)+1);
    glVertex2f(x(ManPositionX+ManHeadRadius)-1,y(ManPositionY)+1);
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY)+2);
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY)+3);

    glVertex2f(x(ManPositionX)+1,y(ManPositionY));
    glVertex2f(x(ManPositionX)+1+1,y(ManPositionY));
    glVertex2f(x(ManPositionX)+1+2,y(ManPositionY));
    glVertex2f(x(ManPositionX)+1+3,y(ManPositionY));
    glVertex2f(x(ManPositionX)+1,y(ManPositionY)+1);
    glVertex2f(x(ManPositionX)+1+1,y(ManPositionY)+1);
    glVertex2f(x(ManPositionX)+1,y(ManPositionY)+2);
    glVertex2f(x(ManPositionX)+1,y(ManPositionY)+3);

    glEnd();

    // Disable antialising
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_BLEND);

    ///SOME LINES
    glBegin(GL_LINES);
    glVertex2f(x(ManPositionX),y(ManPositionY));
    glVertex2f(x(ManPositionX),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));

    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius-ManArmWidth));
    glVertex2f(x(ManPositionX+ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius-ManArmWidth));
    glVertex2f(x(ManPositionX-ManHeadRadius),y(ManPositionY+ManHeight-ManHeadRadius-ManArmHeight));
    glEnd();
}
