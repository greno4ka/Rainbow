#include "scene0.h"

Scene0::Scene0()
{
    X = 800;
    Y = 600;
}

void Scene0::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

void Scene0::setDistance(double newDistance)
{
    distance = newDistance;
}

void Scene0::setWavelength(int newWavelength)
{
    wavelength = newWavelength;
}

void Scene0::setDisplayMode(int newDisplayMode)
{
    displayMode = newDisplayMode;
}

void Scene0::incBeamStep()
{
    if (beamStep < MaxNumberOfReflections)
        beamStep++;
}

void Scene0::resetBeamStep()
{
    beamStep = 0;
}

void Scene0::decBeamStep()
{
    if (beamStep > 0)
    beamStep--;
}

inline double Scene0::x(double x0)
{
    return X-(X/4)+x0*std::min(X/4,Y/2)/SceneScale;
}

inline double Scene0::y(double y0)
{
    return (Y/2)+y0*std::min(X/4,Y/2)/SceneScale;
}

inline double Scene0::r(double r0)
{
    return r0*std::min(X/4,Y/2)/SceneScale;
}

void Scene0::draw_drop()
{
    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    const int DropQuality=50;

    for (int i=0; i<=DropQuality; i++)
    {
        double currentAngle = (double)i/DropQuality*2*M_PI;
        glVertex2f(x(0)+r(Scene0::DropRadius)*cos(currentAngle),
                   y(0)+r(Scene0::DropRadius)*sin(currentAngle));
    }
    glEnd();
}

void Scene0::draw_axes()
{
    glColor3ub(255,255,255);
    glEnable(GL_LINE_STIPPLE); // turn on - - - - - -
    glLineStipple(1, 0x1111);  // 1 , 1111 means tiny dashes
    glBegin(GL_LINES);
    glVertex2f(0,y(0));
    glVertex2f(X,y(0));
    glVertex2f(x(0),0);
    glVertex2f(x(0),Y);
    glEnd();
    glDisable(GL_LINE_STIPPLE); // turn it off
}

void Scene0::draw_beam(Beam beam)
{
    double p=1,     // GAMMA CORRECTOR of color. For dark beams
        x0,y0,      // point0
        x1,y1,      // point1
        x2,y2;      // point2 - external (for reformed)
    Beam Reformed,
        Radius,
        Reflected;

    beam.calculateInputPoint(&x0, &y0);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    glColor3ub(beam.getR(),beam.getG(),beam.getB());
    glBegin(GL_LINES);
    glVertex2f(0,y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();

//    if(N>=0)
//    {
//// First reflection code
//// ONLY IN 0 SCENE
//        Radius.calculateKoeffs(x0,y0,0,0);
//        Reflected=beam;
//        Reflected.reflect(Radius);
//        Reflected.calculateInfintyPoint(&x2,&y2,x0,y0);
//        if (!rainbows)
//        {
//            if (beam.w()==true)
//                glColor3ub(255,255,255);
//            else
//                glColor3ub(r,g,b);
//            glBegin(GL_LINES);
//            glVertex2f(fx(x0),fy(y0));
//            glVertex2f(fx(x2),fy(y2));
//            glEnd();
//        }
//// end

//        Reformed=Radius; // we're get reformed from radius
//        Reformed.snell(beam,k(beam.getWL()));
//        beam=Reformed;
//        glLineWidth(1);

//        beam.calculateOutputPoint(&x1, &y1, x0, y0);
//        /// DRAW EXACT BEAM

//        glBegin(GL_LINES);
//        glVertex2f(fx(x0),fy(y0));
//        glVertex2f(fx(x1),fy(y1));
//        glEnd();
//    }
//    for (int z=1; z<=N; z++)
//    {
//        p-=0.2;
//        Radius.calculateKoeffs(x1,y1,0,0);

//        Reformed=Radius; // we're get reformed from radius again
//        Reformed.snell(beam,1/k(beam.getWL())); // 1/k cause goin' from inside out
//        Reformed.calculateInfintyPoint(&x2,&y2,x1,y1);

//        glColor3ub(r*p,g*p,b*p);
//        glBegin(GL_LINES);
///// DRAW REFORMED BEAM
//       // if(!((z==2) && x2>0)) // x2 - to kill one bug
//            if(!rainbows || (radio_ch==1 && z==2) || (radio_ch==2 && z==3))
//            {
//                    glVertex2f(fx(x1),fy(y1));
//                    glVertex2f(fx(x2),fy(y2));
//            }
//        glEnd();

//        beam.reflect(Radius);
//        x0=x1;
//        y0=y1;
//        beam.calculateOutputPoint(&x1, &y1, x0, y0);
//        /// DRAW EXACT BEAM
//        if(rainbows==0 || !(!rainbows || (radio_ch==1 && z==2) || (radio_ch==2 && z==3)))
//        {
//        glBegin(GL_LINES);
//        glVertex2f(fx(x0),fy(y0));
//        glVertex2f(fx(x1),fy(y1));
//        glEnd();
//        }
//    }
}

void Scene0::display()
{
    draw_drop();
    draw_axes();
    for (Lst::iterator i=Beams.begin(); i!=Beams.end(); i++)
        draw_beam(*i);
}
