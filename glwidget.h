#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QTimer>

#include "scenes/scenebase.h"
#include "scenes/scene0.h"
#include "scenes/scene1.h"
#include "scenes/scene2.h"
#include "scenes/scene3.h"
#include "scenes/scene4.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
    QTimer timer;

    SceneBase *scene;
    Scene0 *scene0;
    Scene1 *scene1;
    Scene2 *scene2;
    Scene3 *scene3;
    Scene4 *scene4;

    int sceneNumber;

public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void connectWithScene0(Scene0 &originalScene0);
    void connectWithScene1(Scene1 &originalScene1);
    void connectWithScene2(Scene2 &originalScene2);
    void connectWithScene3(Scene3 &originalScene3);
    void connectWithScene4(Scene4 &originalScene4);

    void setSceneNumber(int programMode);
};

#endif // GLWIDGET_H
