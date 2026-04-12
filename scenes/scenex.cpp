#include "scenex.h"

SceneX::SceneX()
{
    X = 800;
    Y = 600;
}


void SceneX::display()
{
    // draw grid
    double MAX = 1000; // Length of half of field
    glColor3f(0.5f, 1.0f, 0.5f);
    glBegin(GL_LINES);
    for (double i=-MAX; i<MAX; i++) {
        glVertex3d( i, -MAX, 0.0);
        glVertex3d( i, MAX, 0.0);
        glVertex3d( -MAX, i, 0.0);
        glVertex3d( MAX, i, 0.0);
    }
    glEnd();
}

void SceneX::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}