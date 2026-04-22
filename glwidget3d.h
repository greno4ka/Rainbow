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
    // More Factor -> less sensitivity
    static constexpr double ZoomFactor = 1.0;
    static constexpr double PanFactor = 1000.0;

    QVector3D camera;
    QVector3D target;

    double
        phi,        // horizontal angle
        psy,        // vertical angle
        distance;   // distance of camera from center

    QTimer *timer = nullptr;

    SceneX *scenex;

    int mouse_x, mouse_y; // for mouse tracking
    int mouse_button; // mouse pressed flag: 0 - mouse up, 1 - mouse down
    int key_button;

protected:

    /// GLMainWindow parameters
    int width, height; // GLMainWindow sizes

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void updateCamera();

public:
    explicit GLWidget3D(QWidget *parent = 0);

    void connectWithSceneX(SceneX &originalSceneX);
};

#endif // GLWIDGET3D_H
