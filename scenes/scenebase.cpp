#include "scenebase.h"

#include "beam.h"
#include "wavelength.h"
#include "k.h"

SceneBase::SceneBase()
{
    X = 800;
    Y = 600;
}

void SceneBase::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

double SceneBase::x(double x0)
{
    return X-(X/4)+x0*std::min(X/4,Y/2)/SceneScale;
}

double SceneBase::y(double y0)
{
    return (Y/2)+y0*std::min(X/4,Y/2)/SceneScale;
}

double SceneBase::r(double r0)
{
    return r0*std::min(X/4,Y/2)/SceneScale;
}

void SceneBase::draw_drop()
{
    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);

    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    for (int i=0; i<=DropQuality; i++) {
        double currentAngle = (double)i/DropQuality*2*M_PI;
        glVertex2f(x(0)+r(DropRadius)*cos(currentAngle),
                   y(0)+r(DropRadius)*sin(currentAngle));
    }
    glEnd();

    // Disable antialising
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
}

void SceneBase::draw_axes()
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

void SceneBase::draw1stRainbow(Beam beam)
{
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    Beam refracted,
         radius,
         reflected;

    int r,g,b;
    wavelengthToRGB(beam.getWL(),&r,&g,&b);

    beam.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);

    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    glColor3ub(r,g,b);
    glBegin(GL_LINES);
    glVertex2f(0,y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();

    /// FIRST REFRACTION
    refracted = radius;
    refracted.snell(beam, k(beam.getWL()));
    beam = refracted;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();

    /// REFLECTION INSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();

    /// REFRACTION OUTSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    refracted = radius;
    refracted.snell(beam, 1/k(beam.getWL())); // 1/k because beam is going from inside out
    refracted.calculateInfintyPoint(&x2,&y2,x1,y1);

    glBegin(GL_LINES);
    glVertex2f(x(x1),y(y1));
    glVertex2f(x(x2),y(y2));
    glEnd();

    // Disable antialising
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

void SceneBase::draw2ndRainbow(Beam beam)
{
    double x0,y0,      // point0
           x1,y1,      // point1
           x2,y2;      // point2 - external (for reformed outside)

    Beam refracted,
         radius,
         reflected;

    int r,g,b;
    wavelengthToRGB(beam.getWL(),&r,&g,&b);

    beam.calculateInputPoint(&x0, &y0);
    radius.calculateKoeffs(x0,y0,0,0);

    // Enable antialising
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    /// ORIGINAL BEAM
    // this part should be drawn anyway
    glColor3ub(r,g,b);
    glBegin(GL_LINES);
    glVertex2f(0,y(y0));
    glVertex2f(x(x0),y(y0));
    glEnd();

    /// FIRST REFRACTION
    refracted = radius;
    refracted.snell(beam, k(beam.getWL()));
    beam = refracted;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();

    /// REFLECTION INSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();

    /// NEXT REFLECTION INSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    beam.reflect(radius);
    x0=x1; y0=y1;
    beam.calculateOutputPoint(&x1, &y1, x0, y0);

    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();

    /// REFRACTION OUTSIDE
    radius.calculateKoeffs(x1,y1,0,0);
    refracted = radius;
    refracted.snell(beam, 1/k(beam.getWL())); // 1/k because beam is going from inside out
    refracted.calculateInfintyPoint(&x2,&y2,x1,y1);

    glBegin(GL_LINES);
    glVertex2f(x(x1),y(y1));
    glVertex2f(x(x2),y(y2));
    glEnd();

    // Disable antialising
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}
