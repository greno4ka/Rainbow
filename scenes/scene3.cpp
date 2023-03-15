#include "scene3.h"

Scene3::Scene3()
{
    // double Z=std::min(X/4,Y/2);
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
