#include "scenex.h"

QVector3D SceneX::generateSunPoint()
{
    double randAngle = (double)rand() / RAND_MAX * 2.0 * M_PI;
    double randRadius = sqrt((double)rand() / RAND_MAX) * sunRadius;

    return QVector3D(sunCenter.x(), sunCenter.y() + std::cos(randAngle)*randRadius, sunCenter.z() + std::sin(randAngle)*randRadius);
}

void SceneX::addBeams(int numberOfAddedBeams)
{
    for (int i=0; i < numberOfAddedBeams; i++)
        sunPoints.push_back(generateSunPoint());
    numberOfBeams+=numberOfAddedBeams;
}


SceneX::SceneX()
{
    X = 800;
    Y = 600;

    sunRadius = 10;

    sunCenter = { -100.0, 0.0, 50.0 };
    wallCenter = { 0.0, 0.0, 6.0 };

    raysDirection = wallCenter - sunCenter;
    raysDirection.normalize();

    addBeams(100);

    timer.start();
}


void SceneX::drawSun(double cx, double cy, double cz)
{
    const int segments = 50;

    glColor3f(1.0f, 0.9f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);

    // центр
    glVertex3d(cx, cy, cz);

    for (int i = 0; i <= segments; i++)
    {
        double angle = 2.0 * M_PI * i / segments;
        double y = cy + cos(angle) * sunRadius;
        double z = cz + sin(angle) * sunRadius;

        glVertex3d(cx, y, z);
    }

    glEnd();
}

void SceneX::drawWall()
{
    double wallH = 10.0;
    double wallW = 6.0;

    glColor3f(0.7f, 0.7f, 1.0f);

    double x = wallCenter.x();
    double y0 = wallCenter.y() - wallH;
    double y1 = wallCenter.y() + wallH;
    double z0 = wallCenter.z() - wallW;
    double z1 = wallCenter.z() + wallW;

    glBegin(GL_TRIANGLE_FAN);

    glVertex3d(x, wallCenter.y(), wallCenter.z());

    glVertex3d(x, y0, z0);
    glVertex3d(x, y0, z1);
    glVertex3d(x, y1, z1);
    glVertex3d(x, y1, z0);
    glVertex3d(x, y0, z0);

    glEnd();
}

void SceneX::drawSingleRay(QVector3D sunPoint, double timeSec)
{

    double kSpeed=50;
    double t = kSpeed * std::fmod(timeSec, 10.0);

    // точка без ограничений (если бы стены не было)
    QVector3D q_free = {
        sunPoint.x() + raysDirection.x() * t,
        sunPoint.y() + raysDirection.y() * t,
        sunPoint.z() + raysDirection.z() * t
    };

    QVector3D q = q_free;

    // пересечение со стеной (по X)
    if (std::abs(raysDirection.x()) > 1e-9)
    {
        double t_hit = (wallCenter.x() - sunPoint.x()) / raysDirection.x();

        if (t_hit >= 0.0 && t_hit < t)
        {
            q = {
                sunPoint.x() + raysDirection.x() * t_hit,
                sunPoint.y() + raysDirection.y() * t_hit,
                sunPoint.z() + raysDirection.z() * t_hit
            };
        }
    }

    glColor3f(1.0f, 1.0f, 0.2f);
    glBegin(GL_LINES);
    glVertex3d(sunPoint.x(), sunPoint.y(), sunPoint.z());
    glVertex3d(q.x(), q.y(), q.z());
        glEnd();
}

void SceneX::display()
{
    double t = fmod(timer.elapsed() / 1000.0, 5.0);

    double timeSec = timer.elapsed() / 1000.0;

    double speed = 50.0;

    double dist = speed * t;

    double maxFlight = 10.0 * speed;

    dist = std::min(dist, maxFlight);

    // draw grid
    double MAX = 1000; // Length of half of field
    glColor3f(0.5f, 1.0f, 0.5f);
    glBegin(GL_LINES);
    for (double i=-MAX; i<MAX; i++) {
        glVertex3d( i, -MAX, 0.0);
        glVertex3d( i, MAX, 0.0);
        glVertex3d( -MAX, i, 0.0);
        glVertex3d( MAX, i, 0.0);
    }
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3d( 0, -MAX, 0.0);
    glVertex3d( 0, MAX, 0.0);
    glVertex3d( 0, 0, -MAX);
    glVertex3d( 0, 0, MAX);
    glVertex3d( -MAX, 0, 0.0);
    glVertex3d( MAX, 0, 0.0);

    glEnd();


    drawWall();

    // surface
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d( 0, 0, 0);
    glVertex3d( 0, -10, 0);
    glVertex3d( -20, -10, 0);
    glVertex3d( -20, 10, 0);
    glVertex3d( 0, 10, 0);
    glEnd();

    drawSun(-100.0, 0.0, 50.0);

    for (QVector3Ds::iterator sunPoint=sunPoints.begin(); sunPoint!=sunPoints.end(); sunPoint++)
        drawSingleRay(*sunPoint, timeSec);
}

void SceneX::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}