#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QTimer>


class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

void initializeGL();
void paintGL();
void resizeGL(int w, int h);

private:
QTimer timer;
};

#endif // GLWIDGET_H
