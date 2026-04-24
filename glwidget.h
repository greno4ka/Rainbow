#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "scenes/scenebase.h"
#include "scenes/scene1.h"
#include "scenes/scene2.h"
#include "scenes/scene3.h"
#include "scenes/scene4.h"
#include "scenes/scene5.h"

#include "scenes/scene6.h"
#include "scenes/scene7.h"

#include <QTimer>

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    QTimer *timer = nullptr;

    SceneBase *scene = nullptr;
    Scene1 *scene1 = nullptr;
    Scene2 *scene2 = nullptr;
    Scene3 *scene3 = nullptr;
    Scene4 *scene4 = nullptr;
    Scene5 *scene5 = nullptr;

    Scene6 *scene6 = nullptr;
    Scene7 *scene7 = nullptr;

    int width = 0, height = 0;

    int sceneNumber = 0;

    void timerStart();
    void timerStop();

public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void connectWithScene1(Scene1 &originalScene1);
    void connectWithScene2(Scene2 &originalScene2);
    void connectWithScene3(Scene3 &originalScene3);
    void connectWithScene4(Scene4 &originalScene4);
    void connectWithScene5(Scene5 &originalScene5);

    void connectWithScene6(Scene6 &originalScene6);
    void connectWithScene7(Scene7 &originalScene7);

    void setSceneNumber(int programMode);
};

#endif // GLWIDGET_H
