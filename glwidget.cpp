#include "glwidget.h"
#include "display.h"

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
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
    //Display();
    scene0->display();
}

void GLWidget::resizeGL(int w, int h){
    /// this resize function allows to work in accustomed coordinates
    scene0->updateXY(w, h);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble) w, 0.0, (GLdouble) h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::connectWithScene0(Scene0 &originalScene0)
{
    scene0 = &originalScene0;
}
