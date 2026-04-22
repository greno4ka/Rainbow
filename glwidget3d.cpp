#include "glwidget3d.h"
#include <QOpenGLFunctions>

GLWidget3D::GLWidget3D(QWidget *parent) :
    QOpenGLWidget(parent)
{
    this->setMouseTracking(true);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() { update(); });
    timer->start(1);

    mouse_button=0;

    // note that angles in radians
    phi = DefaultPhi;
    psy = DefaultPsy;
    distance = DefaultDistance;

    camera = { 0.0, 0.0, 0.0 };
    target = DefaultTarget;
    worldUp = {0.0, 0.0, 1.0 };

    startCamera = camera;
    startTarget = target;

    updateCamera();
}

void GLWidget3D::resetCamera()
{
    QVector3D defaultCamera = QVector3D(
        DefaultTarget.x() + cos(DefaultPsy) * sin(DefaultPhi) * DefaultDistance,
        DefaultTarget.y() + cos(DefaultPsy) * cos(DefaultPhi) * DefaultDistance,
        DefaultTarget.z() + sin(DefaultPsy) * DefaultDistance
    );

    resetting = true;

    flyTo(defaultCamera,DefaultTarget);
}

void GLWidget3D::updateCamera()
{
    if (cameraMode == 0) {
        camera = QVector3D(
            target.x() + cos(psy) * sin(phi) * distance,
            target.y() + cos(psy) * cos(phi) * distance,
            target.z() + sin(psy) * distance
        );
    }
    else {
        QVector3D forward(
            -cos(psy) * sin(phi),
            cos(psy) * cos(phi),
            sin(psy)
        );
        camera = scenex->getEye();
        target = camera + forward * distance;
    }
}

void GLWidget3D::flyTo(QVector3D destCamera, QVector3D destTarget)
{
    if (cameraMode == 1) {
        switchCameraMode();
        // In person based camera we use vertical angle in other way
        psy = -psy;
        flyDirection = 0;
    }

    startCamera = camera;
    startTarget = target;

    endCamera = destCamera;
    endTarget = destTarget;

    QVector3D destDistance = destCamera - destTarget;

    endDistance = destDistance.length();
    endPsy = asin(destDistance.z() / destDistance.length());
    endPhi = atan2(destDistance.x(), destDistance.y());

    startPhi = phi;
    startPsy = psy;
    startDistance = distance;

    flyTime = 0.0;
    flying = true;
}

void GLWidget3D::connectWithSceneX(SceneX &originalSceneX)
{
    scenex = &originalSceneX;
}

void GLWidget3D::switchCameraMode()
{
    cameraMode = !cameraMode;
}

bool GLWidget3D::getCameraMode()
{
    return cameraMode;
}

QVector3D GLWidget3D::getLastCamera()
{
    return startCamera;
}

QVector3D GLWidget3D::getLastTarget()
{
    return startTarget;
}

void GLWidget3D::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(
        camera.x(), camera.y(), camera.z(),
        target.x(), target.y(), target.z(),
        worldUp.x(), worldUp.y(), worldUp.z()
    );

    if (flying) {
        flyTime += FlyFrame;
        double t = std::min(flyTime, FlyDuration);

        // smoothstep
        double s = t * t * (3 - 2 * t);

        target = startTarget * (1 - s) + endTarget * s;

        phi = startPhi + (endPhi - startPhi) * s;
        psy = startPsy + (endPsy - startPsy) * s;
        distance = startDistance + (endDistance - startDistance) * s;

        if (t == FlyDuration) {
            flying = false;

            if (flyDirection == 1 && !resetting) {
                switchCameraMode();
                psy = -psy;
                flyDirection = 0;
            } else
                flyDirection = 1;
            if (resetting)
                resetting = false;
        }
    }

    updateCamera();

    scenex->display();

}

void GLWidget3D::resizeGL(int w, int h)
{
    scenex->updateXY(w, h);
    width = w; height = h;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, static_cast<double>(width)/height, 0.01, 11000);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget3D::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        //key_pressed = 1;
        break;
    case Qt::Key_F:
        //key_pressed = 1;
        break;
    }
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->position().x() - mouse_x;
    int dy = event->position().y() - mouse_y;

    if(mouse_button == 1) {
        phi += dx * M_PI / 180.0 / 4.0; // horizontal angle
        psy += dy * M_PI / 180.0 / 4.0; // vertical angle

        phi = std::fmod(phi, 2.0*M_PI);

        psy = std::min(psy, M_PI/2.0);
        psy = std::max(psy, -M_PI/2.0);

        updateCamera();
    }

    if(mouse_button == 2) {
        double panSpeed = distance / PanFactor;

        // Construct camera basis (forward/right/up vectors) from view direction
        QVector3D forward = (target - camera).normalized();
        QVector3D right = QVector3D::crossProduct(forward, worldUp).normalized();
        QVector3D up    = QVector3D::crossProduct(right, forward).normalized();

        QVector3D delta = (-right*dx + up*dy) * panSpeed;

        target += QVector3D(delta.x(), delta.y(), delta.z());

        updateCamera();
    }

    mouse_x = event->position().x();
    mouse_y = event->position().y();
}

void GLWidget3D::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        mouse_button=1;
    if(event->button()==Qt::RightButton)
        mouse_button=2;

    update();
}

void GLWidget3D::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        mouse_button=0;
    if(event->button()==Qt::RightButton)
        mouse_button=0;

    update();
}

void GLWidget3D::wheelEvent(QWheelEvent *event)
{
    // Qt reports wheel delta in units of 1/8 of a degree
    // A typical mouse wheel step is 15 degrees → 15 * 8 = 120 units
    int numDegrees = event->angleDelta().y() / 8;

    distance += numDegrees / ZoomFactor;
    distance = std::max(distance, 0.005);

    updateCamera();

    event->accept();
    update();
}
