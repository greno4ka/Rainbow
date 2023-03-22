#include "scene3.h"

#include "wavelength.h"

Scene3::Scene3()
{
    displayMode = 0;
    dynamicMode = 1;
    desiredFPS = 60;
    currentRainSpeed = 0;
    initialRainSwift = 0;
    currentRainFrame = 0;
    regenerateRain();
    currentRainFrame = 0;
    sunlightPenetration = new double[NumberOfBeams];
}

Scene3::~Scene3()
{
    if (currentRainSpeed) delete [] currentRainSpeed;
    if (initialRainSwift) delete [] initialRainSwift;
    if (currentRainFrame) delete [] currentRainFrame;
    if (sunlightPenetration) delete [] sunlightPenetration;
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

bool Scene3::getDynamicMode() const
{
    return dynamicMode;
}

void Scene3::regenerateRain()
{
    if (currentRainSpeed) delete [] currentRainSpeed;
    if (initialRainSwift) delete [] initialRainSwift;
    if (currentRainFrame) delete [] currentRainFrame;

    cloudBegin = -CloudWidth * r(CloudRadius-1.5) + x(CloudCenterX);
    cloudEnd = CloudWidth * r(CloudRadius-1.5) + x(CloudCenterX);
    numberOfRainDashes = 0;
    for (int i=cloudBegin; i<=cloudEnd; i+=RainStep)
        numberOfRainDashes++;
    currentRainSpeed = new int[numberOfRainDashes+1];
    initialRainSwift = new int[numberOfRainDashes+1];
    currentRainFrame = new int[numberOfRainDashes+1];
    for (int i=0; i<numberOfRainDashes; i++) {
        currentRainSpeed[i] = rand()%3 + 1; // speed should be >0, at least 1
        initialRainSwift[i] = rand()%5;
        currentRainFrame[i] = 0;
    }
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
    double eyeX,eyeY;

    Beam rainEdge(1),
         sunLight(1),
         observed(1);

    if (dynamicMode)
        for (int i=0; i<NumberOfBeams; i++)
            sunlightPenetration[i] = (double)(rand()%500/100.0);

    drawRain();

    eyeX=x(ManPositionX)+6;
    eyeY=y(ManPositionY+ManHeight)+4;

    rainEdge.calculateKoeffs(-1,0,-0.4,10); // - edge of rain \\\ DON'T TOUCH Y - vars!!!

    int beamNumber = 0;
    for (double i=0; i<=10; i+=10.0/(NumberOfBeams-1))
    {
        sunLight.calculateKoeffs(0,i,1,i);

        double dropX, dropY;
        cross_ll(rainEdge, sunLight, &dropX, &dropY);
        dropX+=sunlightPenetration[beamNumber++];

        observed.calculateKoeffs(x(dropX), y(dropY), eyeX, eyeY);
        /// ORIGINAL BEAMS
        glColor3ub(255,255,255);
        glBegin(GL_LINES);
        glVertex2f(0,y(dropY));
        glVertex2f(x(dropX),y(dropY));
        glEnd();

        glEnable(GL_LINE_SMOOTH); // begin of antialiasing
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // end of antialiasing

        if (displayMode == 0)
        {
            for (int wavelength=380; wavelength<=780; wavelength+=60)
            {
                const double LocalInf = 100;
                double x0=dropX-LocalInf;
                double y1=dropY-tan(whatAngle(wavelength,1)*M_PI/180)*LocalInf;
                double y2=dropY-tan(whatAngle(wavelength,2)*M_PI/180)*LocalInf;

                wavelengthToRGB(wavelength,&r,&g,&b);
                glColor3ub(r,g,b);
                glBegin(GL_LINES);
                glVertex2f(x(dropX),y(dropY));
                glVertex2f(x(x0),y(y1));
                glVertex2f(x(dropX),y(dropY));
                glVertex2f(x(x0),y(y2));
                glEnd();
            }
        }
        if (displayMode == 1)
            if ( (observed.getAngle() >= whatAngle(380,1) && observed.getAngle() <= whatAngle(780,1) ) ||
                 (observed.getAngle() >= whatAngle(780,2) && observed.getAngle() <= whatAngle(380,2) )
                 )
            {
                double rd=40,   // rd - mini radius [pixels]
                       xcut,ycut; // cutted beam
                xcut=eyeX+rd*cos(observed.getAngle()*M_PI/180);
                ycut=eyeY+rd*sin(observed.getAngle()*M_PI/180);
                if (observed.getAngle()<=whatAngle(780,1))
                    wavelengthToRGB(whatWave(observed.getAngle(),1),&r,&g,&b);
                else wavelengthToRGB(whatWave(observed.getAngle(),2),&r,&g,&b);
                glBegin(GL_LINES);
                glColor3ub(255,255,255);
                glColor3ub(r,g,b);
                glVertex2f(x(dropX),y(dropY));
                glVertex2f(xcut,ycut);
                glEnd();
                drawMan();
            }
        if (displayMode == 2)
            if ( (observed.getAngle() >= whatAngle(380,1) && observed.getAngle() <= whatAngle(780,1) ) ||
                 (observed.getAngle() >= whatAngle(780,2) && observed.getAngle() <= whatAngle(380,2) )
                 )
            {
                for (int w=380; w<=780; w+=30)
                {
                    double x0=dropX-30;
                    double y1=dropY+tan(whatAngle(w,1)*M_PI/180)*(-30);
                    double y2=dropY+tan(whatAngle(w,2)*M_PI/180)*(-30);

                    wavelengthToRGB(w,&r,&g,&b);
                    glColor3ub(r,g,b);
                    glBegin(GL_LINES);
                    glVertex2f(x(dropX),y(dropY));
                    glVertex2f(x(x0),y(y1));
                    glVertex2f(x(dropX),y(dropY));
                    glVertex2f(x(x0),y(y2));
                    glEnd();
                }
                drawMan();
            }
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }

    drawFloor();
    drawCloud();

    Sleep(888/desiredFPS);

}

void Scene3::drawRain()
{
    glColor3ub(200,200,200);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    glEnable(GL_LINE_STIPPLE); // dashed line
    glLineStipple(15, 0xAAAA); // style of dashes
    glBegin(GL_LINES);

    int dashNumber = 0;
    for (int currentX=cloudBegin; currentX<=cloudEnd; currentX+=RainStep) {
        /// Our rain goes from underground upstairs to the cloud :3
        glVertex2f(currentX + initialRainSwift[dashNumber]
                   -currentRainFrame[dashNumber]*currentRainSpeed[dashNumber]
                   -y(CloudCenterY)/RainKoef ,

                   initialRainSwift[dashNumber]*RainKoef
                   -currentRainFrame[dashNumber]*RainKoef*currentRainSpeed[dashNumber]);

        glVertex2f(currentX ,y(CloudCenterY));

        if (currentRainFrame[dashNumber] < currentRainSpeed[dashNumber]*30)
            currentRainFrame[dashNumber]++;
        else
            currentRainFrame[dashNumber] = 0;
        dashNumber++;
    }

    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

void Scene3::drawCloud()
{
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
