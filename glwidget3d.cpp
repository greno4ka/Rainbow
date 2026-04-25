#include "glwidget3d.h"
#include <QOpenGLFunctions>

GLWidget3D::GLWidget3D(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setMouseTracking(true);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            this,
            static_cast<void (QWidget::*)()>(&QWidget::update));
    timer->start(1);

    mouse_button = 0;

    state.phi = DefaultPhi;
    state.psy = DefaultPsy;
    state.distance = DefaultDistance;

    state.camera = { 0.0, 0.0, 0.0 };
    state.target = DefaultTarget;
    state.worldUp = {0.0, 0.0, 1.0 };

    state.startCamera = state.camera;
    state.startTarget = state.target;

    updateCamera();
}

GLWidget3D::~GLWidget3D()
{
    if (timer) {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
    }
}

GLWidget3DState GLWidget3D::getState() const
{
    return state;
}

void GLWidget3D::setState(const GLWidget3DState &newState)
{
    state = newState;

    updateCamera();
    update();
}

void GLWidget3D::resetCamera()
{
    QVector3D defaultCamera(
        DefaultTarget.x() + cos(DefaultPsy) * sin(DefaultPhi) * DefaultDistance,
        DefaultTarget.y() + cos(DefaultPsy) * cos(DefaultPhi) * DefaultDistance,
        DefaultTarget.z() + sin(DefaultPsy) * DefaultDistance
        );

    state.resetting = true;

    flyTo(defaultCamera, DefaultTarget);
}

void GLWidget3D::updateCamera()
{
    if (state.cameraMode == 0) {
        state.camera = QVector3D(
            state.target.x() + cos(state.psy) * sin(state.phi) * state.distance,
            state.target.y() + cos(state.psy) * cos(state.phi) * state.distance,
            state.target.z() + sin(state.psy) * state.distance
            );
    }
    else {
        QVector3D forward(
            -cos(state.psy) * sin(state.phi),
            cos(state.psy) * cos(state.phi),
            sin(state.psy)
            );

        state.camera = scenex->getEye();
        state.target = state.camera + forward * state.distance;
    }
}

void GLWidget3D::flyTo(QVector3D destCamera, QVector3D destTarget)
{
    if (state.cameraMode == 1) {
        switchCameraMode();
        // In person based camera we use vertical angle in other way
        state.psy = -state.psy;
        state.flyDirection = 0;
    }

    state.startCamera = state.camera;
    state.startTarget = state.target;

    state.endCamera = destCamera;
    state.endTarget = destTarget;

    QVector3D destDistance = destCamera - destTarget;

    state.endDistance = destDistance.length();
    state.endPsy = asin(destDistance.z() / destDistance.length());
    state.endPhi = atan2(destDistance.x(), destDistance.y());

    state.startPhi = state.phi;
    state.startPsy = state.psy;
    state.startDistance = state.distance;

    state.flyTime = 0.0;
    state.flying = true;
}

void GLWidget3D::connectWithSceneX(SceneX &originalSceneX)
{
    scenex = &originalSceneX;
}

void GLWidget3D::switchCameraMode()
{
    state.cameraMode = !state.cameraMode;
}

bool GLWidget3D::getCameraMode()
{
    return state.cameraMode;
}

QVector3D GLWidget3D::getLastCamera()
{
    return state.startCamera;
}

QVector3D GLWidget3D::getLastTarget()
{
    return state.startTarget;
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

    QMatrix4x4 view;
    view.lookAt(state.camera, state.target, state.worldUp);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(view.constData());

    if (state.flying) {
        state.flyTime += FlyFrame;
        double t = std::min(state.flyTime, FlyDuration);

        // smoothstep
        double s = t * t * (3 - 2 * t);

        state.target = state.startTarget * (1 - s) + state.endTarget * s;

        state.phi = state.startPhi + (state.endPhi - state.startPhi) * s;
        state.psy = state.startPsy + (state.endPsy - state.startPsy) * s;
        state.distance = state.startDistance + (state.endDistance - state.startDistance) * s;

        if (t == FlyDuration) {
            state.flying = false;

            if (state.flyDirection == 1 && !state.resetting) {
                switchCameraMode();
                state.psy = -state.psy;
                state.flyDirection = 0;
            } else {
                state.flyDirection = 1;
            }

            if (state.resetting)
                state.resetting = false;
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
    QMatrix4x4 projection;
    projection.perspective(60.0f, float(width)/float(height), 0.01f, 11000.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.constData());
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

    if (mouse_button == 1) {
        state.phi += dx * M_PI / 180.0 / 4.0; // horizontal angle
        state.psy += dy * M_PI / 180.0 / 4.0; // vertical angle

        state.phi = std::fmod(state.phi, 2.0 * M_PI);

        state.psy = std::min(state.psy, M_PI/2.0);
        state.psy = std::max(state.psy, -M_PI/2.0);

        updateCamera();
    }

    if (mouse_button == 2) {
        double panSpeed = state.distance / PanFactor;

        // Construct camera basis (forward/right/up vectors) from view direction
        QVector3D forward = (state.target - state.camera).normalized();
        QVector3D right = QVector3D::crossProduct(forward, state.worldUp).normalized();
        QVector3D up = QVector3D::crossProduct(right, forward).normalized();

        QVector3D delta = (-right*dx + up*dy) * panSpeed;

        state.target += delta;
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
    int numDegrees = event->angleDelta().y() / 8;

    state.distance += numDegrees / ZoomFactor;
    state.distance = std::max(state.distance, 0.005);

    updateCamera();

    event->accept();
    update();
}