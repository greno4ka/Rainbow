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

    static constexpr double FlyDuration = 1.0; // 1 second
    static constexpr double FlyFrame = 0.01 ;

    static constexpr double DefaultPhi = -M_PI*5.0/8.0; // horizontal angle
    static constexpr double DefaultPsy = M_PI/8.0;      // vertical angle
    static constexpr double DefaultDistance = 400;      // distance of camera from center

    bool cameraMode = 0;

    /// Static camera stuff
    QVector3D camera;
    QVector3D target;
    QVector3D worldUp;

    double
        phi,        // horizontal angle
        psy,        // vertical angle
        distance;   // distance of camera from center

    /// Flying camera stuff
    bool flying = false;
    bool resetting = false; // special type of flight

    QVector3D startCamera, endCamera;
    QVector3D startTarget, endTarget;
    double startPhi, startPsy, startDistance;
    double endPhi, endPsy, endDistance;

    double flyTime = 0.0;
    bool flyDirection = 1;


    QTimer *timer = nullptr;

    SceneX *scenex;

    int mouse_x, mouse_y; // for mouse tracking
    int mouse_button; // mouse pressed flag: 0 - mouse up, 1 - mouse down
    int key_button;

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
    static constexpr QVector3D DefaultTarget = { 0.0, 0.0, 50.0 };

    explicit GLWidget3D(QWidget *parent = 0);

    void flyTo(QVector3D destCamera, QVector3D destTarget);
    void switchCameraMode();
    bool getCameraMode();
    QVector3D getLastCamera();
    QVector3D getLastTarget();

    void resetCamera();

    void connectWithSceneX(SceneX &originalSceneX);
};

#endif // GLWIDGET3D_H
