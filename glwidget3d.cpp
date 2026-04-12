#include "glwidget3d.h"
#include <QOpenGLFunctions>

GLWidget3D::GLWidget3D()
{
    this->setMouseTracking(true);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

    mouse_button=0;

    // note that angles in radians
    phi = -2.5;         // horizontal angle
    psy = 1.0;         // vertical angle
    distance = 30;      // distance of camera from center
    ox = -5; oy = -5; oz = -1; // translate point which we see from center
    cx = cos(psy) * sin(phi) * distance;    // camera coordinates
    cy = cos(psy) * cos(phi) * distance; // camera coordinates
    cz = sin(psy) * distance; // camera coordinates
}

void GLWidget3D::initializeGL(){
    glColor3f(1,1,1);
//    glClearColor(Qt::white);
//    setFormat(GLFormat(GL::DoubleBuffer)); // Двойная буферизация
    glDepthFunc(GL_LEQUAL); // Буфер глубины
}

void GLWidget3D::paintGL(){
    display();
}

void GLWidget3D::resizeGL(int w, int h){
    width = w; height = h;
}

void GLWidget3D::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
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

        psy = std::min(psy, M_PI/2.0);
        psy = std::max(psy, -M_PI/2.0);

        cx = cos(psy) * sin(phi) * distance;
        cy = cos(psy) * cos(phi) * distance;
        cz = sin(psy) * distance;
    }

    if(mouse_button == 2) {
        ox -= cos(phi)*dx/20;
        oy += sin(phi)*dx/20;
        oz -= sin(psy)*dy/5;
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
    distance += numDegrees / 15;
    distance = std::max(distance, 0.005);

    cx = cos(psy) * sin(phi) * distance;
    cy = cos(psy) * cos(phi) * distance;
    cz = sin(psy) * distance;

    event->accept();
    update();
}
