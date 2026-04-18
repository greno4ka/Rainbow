#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() { update(); });

    sceneNumber = 6;
}

void GLWidget::timerStart()
{
    int interval = 1000 / scene4->getDesiredFPS();
    timer->start(interval);
}

void GLWidget::timerStop()
{
    timer->stop();
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
        timerStart();
        break;
    case 5:
        scene = scene5;
        break;
    case 6:
        scene = scene6;
        break;
    default:
        scene = scene6;
        break;
    }
    connect(scene, &SceneBase::requestUpdate, this, [this]() { update(); });
    connect(scene4, &Scene4::timerStart, this, &GLWidget::timerStart);
    connect(scene4, &Scene4::timerStop, this, &GLWidget::timerStop);

    this->update();
}

void GLWidget::initializeGL()
{
        glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    scene->display();

    QPainter painter(this);
    QColor fontColor = palette().color(QPalette::WindowText);
    QFont font = painter.font();
    font.setPointSize(font.pointSize() * 2);
    painter.setFont(font);
    painter.setPen(fontColor);
    if (sceneNumber == 3 && scene3->getShowAngle()) {
        painter.drawText(scene3->getCoordX(), scene3->getCoordY(),
                         QString::number(scene3->getCurrentAngle(), 'f', 2));
    }
    if (sceneNumber == 6) {
        painter.drawText(scene6->getCoordX(-4.5), scene6->getCoordY(10),
                         QString("θ₁"));
        painter.drawText(scene6->getCoordX(3), scene6->getCoordY(-4.5),
                         QString("θ₂"));
        painter.drawText(scene6->getCoordX(-15), scene6->getCoordY(7),
                         QString("n₁"));
        painter.drawText(scene6->getCoordX(-15), scene6->getCoordY(-7),
                         QString("n₂"));
    }
}

void GLWidget::resizeGL(int w, int h)
{
    /// this resize function allows to work in accustomed coordinates
    scene1->updateXY(w, h);
    scene2->updateXY(w, h);
    scene3->updateXY(w, h);
    scene4->updateXY(w, h);
    scene5->updateXY(w, h);
    scene6->updateXY(w, h);

    if (sceneNumber == 4)
        scene4->regenerateRain();

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble) w, 0.0, (GLdouble) h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    this->update();
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

void GLWidget::connectWithScene6(Scene6 &originalScene6)
{
    scene6 = &originalScene6;
}
