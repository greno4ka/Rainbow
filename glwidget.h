#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QTimer>

#include "scenes/scene0.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    Scene0 *scene0;

    void connectWithScene0(Scene0 &originalScene0);

private:
    QTimer timer;
};

#endif // GLWIDGET_H
