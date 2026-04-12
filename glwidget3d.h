#ifndef GLWIDGET3D_H
#define GLWIDGET3D_H

#include <QWheelEvent>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QTimer>

#include <GL/glu.h> // for gluPerspective

#include "scenes/scenex.h"

class GLWidget3D : public QOpenGLWidget
{
    Q_OBJECT

private:
    QTimer *timer = nullptr;

    SceneX *scenex;

protected:
    int mouse_x, mouse_y; // for mouse tracking
    int mouse_button; // mouse pressed flag: 0 - mouse up, 1 - mouse down
    int key_button;

    /// GLMainWindow parameters
    int width, height; // GLMainWindow sizes
    double
    phi,        // horizontal angle
    psy,        // vertical angle
    distance,   // distance of camera from center
    ox, oy, oz, // translate point which we see from center
    cx, cy, cz; // camera coordinates

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    explicit GLWidget3D(QWidget *parent = 0);

    void connectWithSceneX(SceneX &originalSceneX);
};

#endif // GLWIDGET3D_H
