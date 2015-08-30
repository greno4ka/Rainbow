#include "glwidget.h"
#include "display.h"
#include <GL/glu.h>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    if (mode==3)
        timer.start(50);
    else
        timer.start(1);
}
void GLWidget::initializeGL(){
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,0);
    Display();
}

void GLWidget::resizeGL(int w, int h){
    /// this resize function allows to work in accustomed coordinates
        X=w;
        Y=h;

        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0,w,0,h);

        glMatrixMode(GL_MODELVIEW);
}
