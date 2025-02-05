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
    case 1:
        scene = scene1;
        break;
    case 2:
        scene = scene2;
        break;
    case 3:
        scene = scene3;
        break;
    case 4:
        scene4->regenerateRain();
        scene = scene4;
        break;
    case 5:
        scene = scene5;
        break;
    default:
        scene = scene1;
        break;
    }

    this->update();
}

void GLWidget::initializeGL(){
}

void GLWidget::paintGL(){
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,0);
    scene->display();

    if (sceneNumber == 2 && scene3->getShowAngle()) {
        QPainter painter(this);
        QColor fontColor = QColor(255,255,255);
        QFont font = painter.font();
        font.setPointSize(font.pointSize() * 2);
        painter.setFont(font);
        painter.setPen(fontColor);
        painter.drawText(scene3->getCoordX(), scene3->getCoordY(),
                         QString::number(scene3->getCurrentAngle()));
    }
}

void GLWidget::resizeGL(int w, int h){
    /// this resize function allows to work in accustomed coordinates
    scene1->updateXY(w, h);
    scene2->updateXY(w, h);
    scene3->updateXY(w, h);
    scene4->updateXY(w, h);
    scene5->updateXY(w, h);

    if (sceneNumber == 4)
        scene4->regenerateRain();

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble) w, 0.0, (GLdouble) h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
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

void GLWidget::connectWithScene5(Scene5 &originalScene5)
{
    scene5 = &originalScene5;
}
