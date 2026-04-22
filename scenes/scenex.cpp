#include "scenex.h"

void SceneX::setShowBeams(int newShowBeams)
{
    showBeams = newShowBeams;
}

void SceneX::setPolychromatic(bool newPolychromatic)
{
    isPolychromatic = newPolychromatic;
}

void SceneX::setSunHeight(int newSunHeight)
{
    sunCenter = { -SunDistance, 0.0, newSunHeight };
    raysDirection = raysDestination - sunCenter;
    raysDirection.normalize();
    reInitializeScene();
}

void SceneX::setEyeHeight(int newEyeHeight)
{
    eyeCenter = { -EyeDistance, 0.0, newEyeHeight };
    reInitializeScene();
}


QVector3D SceneX::getEye()
{
    return eyeCenter;
}

QVector3D SceneX::generateSunPoint()
{
    double randAngle = (double)rand() / RAND_MAX * 2.0 * M_PI;
    double randRadius = sqrt((double)rand() / RAND_MAX) * SunRadius;

    return QVector3D(sunCenter.x(), sunCenter.y() + std::cos(randAngle)*randRadius, sunCenter.z() + std::sin(randAngle)*randRadius);
}

void SceneX::addSunPoints(int numberOfPoints)
{
    for (int i=0; i < numberOfPoints; i++)
        sunPoints.push_back(generateSunPoint());
    calculateSpherePoints();
    //calculateWallPoints();
}


void SceneX::addNumberOfBeams(int newNumberOfBeams){
    totalNumberOfBeams+=newNumberOfBeams;
}

void SceneX::calculateWallPoints()
{
    wallPoints.clear();
    for (QVector3Ds::iterator sunPoint=sunPoints.begin(); sunPoint!=sunPoints.end(); sunPoint++)
        wallPoints.push_back(*sunPoint + raysDirection * ((raysDestination.x() - sunPoint->x()) / raysDirection.x()));
}

void SceneX::reInitializeScene()
{
    sunPoints.clear();
    wallPoints.clear();
    addSunPoints(totalNumberOfBeams);
}

void SceneX::addBeams()
{

}

void SceneX::resetScene()
{
    totalNumberOfBeams = 0;
    sunPoints.clear();
    wallPoints.clear();
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

    sunCenter = { -SunDistance, 0.0, 150.0 };
    eyeCenter = { -EyeDistance, 0.0, 1.0 };

    raysDestination = { 0.0, 0.0, 0.0 };
    raysDirection = raysDestination - sunCenter;
    raysDirection.normalize();

    showBeams = true;

    addSunPoints(1000);
    totalNumberOfBeams = 1000;

    timer.start();
}


void SceneX::drawSun()
{
    glColor3ub(SunColor.x(), SunColor.y(), SunColor.z());
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(sunCenter.x(), sunCenter.y(), sunCenter.z());
    for (int i = 0; i <= ImageQuality; i++) {
        double angle = 2.0 * M_PI * i / ImageQuality;
        double y = sunCenter.y() + cos(angle) * SunRadius;
        double z = sunCenter.z() + sin(angle) * SunRadius;
        glVertex3d(sunCenter.x(), y, z);
    }
    glEnd();
}

void SceneX::drawWall()
{
    double wallW = 200.0;
    double wallH = 200.0;
    double wallU = 20.0;

    glColor3f(0.7f, 0.7f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);

    glVertex3d(0.0,0.0,0.0);

    glVertex3d(0, -wallW, -wallU);
    glVertex3d(0, -wallW, wallH);
    glVertex3d(0, wallW, wallH);
    glVertex3d(0, wallW, -wallU);
    glVertex3d(0, -wallW, -wallU);

    glEnd();
}

void SceneX::drawSingleRay(const QVector3D& sunPoint,
                           const QVector3D& wallPoint,
                           const QVector3D color,
                           double timeSec)
{
    QVector3D beamPath = (wallPoint - sunPoint);
    double len = beamPath.length();

    if (len < 1e-9)
        return;

    beamPath /= len;

    double travelTime = 2.0; // секунды на полный путь

    double progress = std::min(timeSec / travelTime, 1.0);
    double dist = len * progress;

    QVector3D movingPoint = sunPoint + beamPath * dist;

    glColor3ub(color.x(), color.y(), color.z());
    glBegin(GL_LINES);
    glVertex3d(sunPoint.x(), sunPoint.y(), sunPoint.z());
    glVertex3d(movingPoint.x(), movingPoint.y(), movingPoint.z());
    glEnd();
}


void SceneX::drawRay(QVector3D& startPoint, QVector3D& endPoint, QVector3D color)
{
    glLineWidth(2.0f);
    glColor3ub(color.x(), color.y(), color.z());
    glBegin(GL_LINES);
    glVertex3d(startPoint.x(), startPoint.y(), startPoint.z());
    glVertex3d(endPoint.x(), endPoint.y(), endPoint.z());
    glEnd();
    glLineWidth(1.0f);
}

void SceneX::calculateSpherePoints()
{
    wallPoints.clear();

    QVector3D center(-200.0f, 0.0f, 0.0f);
    float R = 200.0f;

    QVector3D dir = raysDirection.normalized();

    for (int i = 0; i < sunPoints.size(); ++i)
    {
        QVector3D sunPoint = sunPoints[i];

        QVector3D oc = sunPoint - center;

        float a = QVector3D::dotProduct(dir, dir);
        float b = 2.0f * QVector3D::dotProduct(oc, dir);
        float c = QVector3D::dotProduct(oc, oc) - R * R;

        float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f)
            continue;

        float sqrtD = std::sqrt(discriminant);

        float t1 = (-b - sqrtD) / (2.0f * a);
        float t2 = (-b + sqrtD) / (2.0f * a);

        float t = -1.0f;

        if (t1 > 0.0f && t2 > 0.0f)
            t = std::max(t1, t2); // берём дальнюю точку
        else if (t1 > 0.0f)
            t = t1;
        else if (t2 > 0.0f)
            t = t2;

        if (t < 0.0f)
            continue;

        QVector3D hitPoint = sunPoint + dir * t;

        wallPoints.push_back(hitPoint);
    }
}

void SceneX::drawWaterSpherePatch()
{
    float R = 300.0f;
    QVector3D center(-300,0,0);

    int stacks = 30;
    int slices = 60;

    // ограничиваем кусок сферы (не вся!)
    float phiMin = -0.0f;  // вниз
    float phiMax =  0.5f;  // вверх

    float thetaMin = -1.0f; // влево
    float thetaMax =  1.0f; // вправо

    glColor3f(0.7f, 0.7f, 1.0f);

    for (int i = 0; i < stacks; i++)
    {
        float phi0 = phiMin + (phiMax - phiMin) * i / stacks;
        float phi1 = phiMin + (phiMax - phiMin) * (i + 1) / stacks;

        glBegin(GL_QUAD_STRIP);

        for (int j = 0; j <= slices; j++)
        {
            float theta = thetaMin + (thetaMax - thetaMin) * j / slices;

            // точка 1
            float x0 = cos(phi0) * cos(theta);
            float y0 = cos(phi0) * sin(theta);
            float z0 = sin(phi0);

            glVertex3f(
                center.x() + R * x0,
                center.y() + R * y0,
                center.z() + R * z0
                );

            // точка 2
            float x1 = cos(phi1) * cos(theta);
            float y1 = cos(phi1) * sin(theta);
            float z1 = sin(phi1);

            glVertex3f(
                center.x() + R * x1,
                center.y() + R * y1,
                center.z() + R * z1
                );
        }

        glEnd();
    }
}

void SceneX::display()
{
    double timeSec = timer.elapsed() / 1000.0;

    drawGrid();
    drawWaterSpherePatch();
    //drawWall();
    drawSurface();
    drawSphere(sunCenter, SunRadius, SunColor, 50, 50);

    int count = std::min(sunPoints.size(), wallPoints.size());

    for (int i = 0; i < count; i++) {
        if (showBeams)
            drawRay(sunPoints[i], wallPoints[i], SunColor);

        double phi = calculateAngle(sunCenter, wallPoints[i], eyeCenter);

        if ((phi >= whatAngle(380,1) && phi <= whatAngle(780,1)) ||
            (phi >= whatAngle(780,2) && phi <= whatAngle(380,2)))
        {
            int r, g, b;

            if (phi <= whatAngle(780,1))
                wavelengthToRGB(whatWave(phi,1), &r, &g, &b);
            else
                wavelengthToRGB(whatWave(phi,2), &r, &g, &b);

            //drawSphere(wallPoints[i], 0.5f, QVector3D(r,g,b), 6, 8);
            glPointSize(5.0f);
            glEnable(GL_POINT_SMOOTH);
            glColor3ub(r,g,b);
            glBegin(GL_POINTS);
            glVertex3d(wallPoints[i].x(),
                       wallPoints[i].y(),
                       wallPoints[i].z());
            glEnd();

            if (showBeams)
                drawRay(wallPoints[i], eyeCenter, QVector3D(r,g,b));
        }
    }

}

void SceneX::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}

void SceneX::drawSphere(const QVector3D& center, float radius, const QVector3D& color, int stacks, int slices)
{
    glColor3ub(color.x(), color.y(), color.z());

    for (int i = 0; i < stacks; i++) {
        float lat0 = M_PI * (-0.5f + (float)i / stacks);
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / stacks);

        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float lng = 2 * M_PI * j / slices;
            float x = cos(lng);
            float y = sin(lng);

            glVertex3f(
                center.x() + radius * x * zr0,
                center.y() + radius * y * zr0,
                center.z() + radius * z0
                );

            glVertex3f(
                center.x() + radius * x * zr1,
                center.y() + radius * y * zr1,
                center.z() + radius * z1
                );
        }
        glEnd();
    }
}

void SceneX::drawGrid()
{
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
}

void SceneX::drawSurface()
{
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d( 0, 0, 0);
    glVertex3d( 0, -200, 0);
    glVertex3d( -200, -200, 0);
    glVertex3d( -200, 200, 0);
    glVertex3d( 0, 200, 0);
    glEnd();
}
