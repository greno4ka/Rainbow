#include "scenex.h"

QVector3D SceneX::generateSunPoint()
{
    double randAngle = (double)rand() / RAND_MAX * 2.0 * M_PI;
    double randRadius = sqrt((double)rand() / RAND_MAX) * sunRadius;

    return QVector3D(sunCenter.x(), sunCenter.y() + std::cos(randAngle)*randRadius, sunCenter.z() + std::sin(randAngle)*randRadius);
}

void SceneX::addSunPoints(int numberOfPoints)
{
    for (int i=0; i < numberOfPoints; i++)
        sunPoints.push_back(generateSunPoint());
}

void SceneX::calculateWallPoints()
{
    wallPoints.clear();
    for (QVector3Ds::iterator sunPoint=sunPoints.begin(); sunPoint!=sunPoints.end(); sunPoint++)
        wallPoints.push_back(*sunPoint + raysDirection * ((wallCenter.x() - sunPoint->x()) / raysDirection.x()));
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

    addSunPoints(1000);
    calculateWallPoints();

    timer.start();
}


void SceneX::drawSun(double cx, double cy, double cz)
{
    const int segments = 50;

    glColor3f(1.0f, 0.9f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(cx, cy, cz);
    for (int i = 0; i <= segments; i++) {
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

void SceneX::drawSingleRay(const QVector3D& sunPoint,
                           const QVector3D& wallPoint,
                           double timeSec)
{
    double kSpeed = 100.0;
    double t = kSpeed * std::fmod(timeSec, 10.0);

    QVector3D dir = (wallPoint - sunPoint);
    double len = dir.length();

    if (len < 1e-9)
        return;

    dir /= len;

    double dist = std::min(t, len);

    QVector3D movingPoint = sunPoint + dir * dist;

    glColor3f(1.0f, 1.0f, 0.2f);
    glBegin(GL_LINES);

    glVertex3d(sunPoint.x(), sunPoint.y(), sunPoint.z());
    glVertex3d(movingPoint.x(), movingPoint.y(), movingPoint.z());

    glEnd();
}

void SceneX::display()
{
    double timeSec = timer.elapsed() / 1000.0;

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

    for (int i = 0; i < sunPoints.size(); i++)
    {
        drawSingleRay(sunPoints[i], wallPoints[i], timeSec);
    }
}

void SceneX::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}