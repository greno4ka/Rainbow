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
    QVector3D worldUp;

    bool flying = false;

    QVector3D startCam, endCam;
    QVector3D startTarget, endTarget;
    double startPhi, startPsy, startDistance;
    double endPhi, endPsy, endDistance;

    double flyTime = 0.0;
    double flySpeed = 0.01;

    double
        phi,        // horizontal angle
        psy,        // vertical angle
        distance;   // distance of camera from center

    QTimer *timer = nullptr;

    SceneX *scenex;

    int mouse_x, mouse_y; // for mouse tracking
    int mouse_button; // mouse pressed flag: 0 - mouse up, 1 - mouse down
    int key_button;

    bool cameraMode = 0;

    void updateCamera();

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

public:
    explicit GLWidget3D(QWidget *parent = 0);

    void flyTo(QVector3D destCamera, QVector3D destTarget);
    void switchCameraMode();

    void connectWithSceneX(SceneX &originalSceneX);
};

#endif // GLWIDGET3D_H
