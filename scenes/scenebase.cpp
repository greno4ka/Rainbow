#include "scenebase.h"

SceneBase::SceneBase(QObject *parent)
    : QObject(parent)
{
    X = 800;
    Y = 600;

    sceneScale = 10.0;

    offsetXFactor = 4.0;
    offsetYFactor = 2.0;

    scaleXFactor = 4.0;
    scaleYFactor = 4.0;
}

void SceneBase::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

void SceneBase::setTheme(bool newDarkThemeEnabled)
{
    darkThemeEnabled = newDarkThemeEnabled;
}

void SceneBase::drawDrop()
{
    glColor3ub(100,100,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x(0),y(0));

    for (int i=0; i<=ImageQuality; i++) {
        double currentAngle = (double)i/ImageQuality*2*M_PI;
        glVertex2f(x(0)+r(DropRadius)*cos(currentAngle),
                   y(0)+r(DropRadius)*sin(currentAngle));
    }
    glEnd();
}

void SceneBase::drawAxes()
{
    if (darkThemeEnabled)
        glColor3ub(255,255,255);
    else
        glColor3ub(0,0,0);

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

void SceneBase::drawRay(double x0, double y0, double x1, double y1)
{
    glBegin(GL_LINES);
    glVertex2f(x(x0),y(y0));
    glVertex2f(x(x1),y(y1));
    glEnd();
}

void SceneBase::drawInitialRay(double x1, double y1)
{
    glBegin(GL_LINES);
    glVertex2f(0,y(y1));
    glVertex2f(x(x1),y(y1));
    glEnd();
}

void SceneBase::drawAngleArc(Beam &beam1, Beam &beam2, double x0, double y0, double radius, bool arcOnTop)
{
    /// beam2 is usually a normal

    double a1 = beam1.getAngle();
    double a2 = beam2.getAngle();

    if(!arcOnTop) {
        a1 -= 180.0;
        a2 -= 180.0;
    }

    double ang1 = a1 * M_PI / 180.0;
    double ang2 = a2 * M_PI / 180.0;

    double d = ang2 - ang1;

    // force small arc
    if (fabs(d) > M_PI/2) {
        if (d > 0) d -= M_PI;
        else       d += M_PI;
    }

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= ImageQuality; i++) {
        double step = (double)i / ImageQuality;
        double ang = ang1 + d * step;

        double x1 = x0 + cos(ang) * radius;
        double y1 = y0 + sin(ang) * radius;

        glVertex2f(x(x1), y(y1));
    }
    glEnd();
}