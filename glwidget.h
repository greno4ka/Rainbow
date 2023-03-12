#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QTimer>

#include "scenes/scene0.h"
#include "scenes/scene1.h"
#include "scenes/scene2.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
    QTimer timer;

    Scene0 *scene0;
    Scene1 *scene1;
    Scene2 *scene2;

public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void connectWithScene0(Scene0 &originalScene0);
    void connectWithScene1(Scene1 &originalScene1);
    void connectWithScene2(Scene2 &originalScene2);
};

#endif // GLWIDGET_H
