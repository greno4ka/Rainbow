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

double SceneX::calculateAngle(QVector3D& sunPoint, QVector3D& wallPoint, QVector3D& eyePoint)
{
    QVector3D v1 = (sunPoint - wallPoint).normalized();
    QVector3D v2 = (eyePoint - wallPoint).normalized();

    double cosTheta = QVector3D::dotProduct(v1, v2);

    double angleRad = std::acos(cosTheta);

    return angleRad * 180.0 / M_PI;
}

SceneX::SceneX()
{
    X = 800;
    Y = 600;

    sunRadius = 10;

    sunCenter = { -100.0, 0.0, 20.0 };
    wallCenter = { 0.0, 0.0, 6.0 };
    eyeCenter = { -10.0, 0.0, 0.0 };

    raysDirection = wallCenter - sunCenter;
    raysDirection.normalize();

    addSunPoints(1000);
    calculateWallPoints();

    timer.start();
}


void SceneX::drawSun()
{
    glColor3ub(SunColor.x(), SunColor.y(), SunColor.z());
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(sunCenter.x(), sunCenter.y(), sunCenter.z());
    for (int i = 0; i <= ImageQuality; i++) {
        double angle = 2.0 * M_PI * i / ImageQuality;
        double y = sunCenter.y() + cos(angle) * sunRadius;
        double z = sunCenter.z() + sin(angle) * sunRadius;
        glVertex3d(sunCenter.x(), y, z);
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
                           const QVector3D color,
                           double timeSec)
{
    double kSpeed = 100.0;
    double t = kSpeed * std::fmod(timeSec, 10.0);

    QVector3D beamPath = (wallPoint - sunPoint);
    double len = beamPath.length();

    if (len < 1e-9)
        return;

    beamPath /= len;

    double dist = std::min(t, len);

    QVector3D movingPoint = sunPoint + beamPath * dist;

    glColor3ub(color.x(), color.y(), color.z());
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

    drawSun();

    glLineWidth(2.0f);
    for (int i = 0; i < sunPoints.size(); i++) {
        drawSingleRay(sunPoints[i], wallPoints[i], SunColor, timeSec);
        double phi = calculateAngle(sunPoints[i], wallPoints[i],eyeCenter);
        if ((phi >= whatAngle(380,1) && phi <= whatAngle(780,1)) || (phi >= whatAngle(780,2) && phi <= whatAngle(380,2)))
        {
            int r, g, b;
            if (phi<=whatAngle(780,1))
                wavelengthToRGB(whatWave(phi,1),&r,&g,&b);
            else wavelengthToRGB(whatWave(phi,2),&r,&g,&b);
            drawSingleRay(wallPoints[i], eyeCenter, QVector3D(r,g,b), timeSec);
        }
    }

    glLineWidth(1.0f);
}

void SceneX::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}