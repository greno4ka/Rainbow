#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
        timer.start(1);
}


void GLWidget::setSceneNumber(int programMode)
{
    sceneNumber = programMode;

    switch (programMode) {
    case 0:
        scene = scene0;
        break;
    case 1:
        scene = scene1;
        break;
    case 2:
        scene = scene2;
        break;
    case 3:
        scene3->regenerateRain();
        scene = scene3;
        break;
    case 4:
        scene = scene4;
        break;
    default:
        break;
    }

    this->update();
}

void GLWidget::initializeGL(){
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,0);
    scene->display();

    if (sceneNumber == 2 && scene2->getShowAngle()) {
        QPainter painter(this);
        QColor fontColor = QColor(255,255,255);
        QFont font = painter.font();
        font.setPointSize(font.pointSize() * 2);
        painter.setFont(font);
        painter.setPen(fontColor);
        painter.drawText(scene2->getCoordX(), scene2->getCoordY(),
                         QString::number(scene2->getCurrentAngle()));
    }
}

void GLWidget::resizeGL(int w, int h){
    /// this resize function allows to work in accustomed coordinates
    scene0->updateXY(w, h);
    scene1->updateXY(w, h);
    scene2->updateXY(w, h);
    scene3->updateXY(w, h);
    scene4->updateXY(w, h);

    if (sceneNumber == 3)
        scene3->regenerateRain();

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

void GLWidget::connectWithScene1(Scene1 &originalScene1)
{
    scene1 = &originalScene1;
}

void GLWidget::connectWithScene2(Scene2 &originalScene2)
{
    scene2 = &originalScene2;
}

void GLWidget::connectWithScene3(Scene3 &originalScene3)
{
    scene3 = &originalScene3;
}

void GLWidget::connectWithScene4(Scene4 &originalScene4)
{
    scene4 = &originalScene4;
}
