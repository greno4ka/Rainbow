#include "scenex.h"

#include <QtConcurrent/QtConcurrent>

#include <iostream>

void SceneX::buildLUT(LUT *lut, int rainbowMode)
{
    lut->clear();

    for (double wavelength = 380; wavelength <= 780; wavelength += 0.5)
    {
        int r, g, b;
        wavelengthToRGB(wavelength, &r, &g, &b);

        double cosA = std::cos(whatAngle(wavelength, rainbowMode)*M_PI/180.0);

        std::sort(lut->begin(), lut->end(),
                  [](const LUTEntry &a, const LUTEntry &b) {
                      return a.cosA < b.cosA;
                  });

        lut->append({cosA, QVector3D(r, g, b)});
    }
}
QVector3D SceneX::getColorFromCosAngle(const LUT &lut, double cosA, int rainbowMode)
{
    double t;
    if (rainbowMode == 1)
        t = (cosA - ca780) / (ca380 - ca780);
    else
        t = (cosA - cb380) / (cb780 - cb380);

    int idx = int(t * (lut.size() - 1));

    //if (idx < 0) idx = 0;
    //if (idx >= lut.size()) idx = lut.size() - 1;

    return lut[idx].color;
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


    timer.start();

    ca380 = std::cos(whatAngle(380, 1) * M_PI / 180.0);
    ca780 = std::cos(whatAngle(780, 1) * M_PI / 180.0);
    cb780 = std::cos(whatAngle(780, 2) * M_PI / 180.0);
    cb380 = std::cos(whatAngle(380, 2) * M_PI / 180.0);

    buildLUT(&lut1,1);
    buildLUT(&lut2,2);


    addSunPoints(1000);
    totalNumberOfBeams = 1000;
}


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
    static std::mt19937 rng(12345);
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    double u1 = dist(rng);
    double u2 = dist(rng);

    double randAngle = u1 * 2.0 * M_PI;
    double randRadius = std::sqrt(u2) * SunRadius;

    double cx = std::cos(randAngle) * randRadius;
    double sy = std::sin(randAngle) * randRadius;

    return QVector3D(
        sunCenter.x(),
        sunCenter.y() + cx,
        sunCenter.z() + sy
        );
}

void SceneX::addSunPoints(int numberOfPoints)
{
    for (int i=0; i < numberOfPoints; i++)
        sunPoints.push_back(generateSunPoint());
    calculateSpherePoints();
}


void SceneX::addNumberOfBeams(int newNumberOfBeams)
{
    totalNumberOfBeams+=newNumberOfBeams;
}

void SceneX::reInitializeScene()
{
    sunPoints.clear();
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
    const double sphereRadius = -eyeCenter.x();
    const QVector3D sphereCenter(eyeCenter.x(), eyeCenter.y(), eyeCenter.z());

    const double ca380 = std::cos(whatAngle(380, 1) * M_PI / 180.0);
    const double ca780 = std::cos(whatAngle(780, 1) * M_PI / 180.0);
    const double cb780 = std::cos(whatAngle(780, 2) * M_PI / 180.0);
    const double cb380 = std::cos(whatAngle(380, 2) * M_PI / 180.0);

    QList<Vertex> localResult;

    const double r2 = sphereRadius * sphereRadius;

    QList<std::optional<Vertex> > results =
        QtConcurrent::blockingMapped(sunPoints,
                                     [&](const QVector3D &sp) -> std::optional<Vertex>
                                     {
                                         QVector3D oc = sp - sphereCenter;

                                         double b = 2.0 * QVector3D::dotProduct(oc, raysDirection);
                                         double c = QVector3D::dotProduct(oc, oc) - r2;

                                         double discriminant = b * b - 4.0 * c;
                                         if (discriminant < 0.0)
                                             return std::nullopt;

                                         double sqrtD = std::sqrt(discriminant);

                                         double t1 = (-b - sqrtD) * 0.5;
                                         double t2 = (-b + sqrtD) * 0.5;
                                         double t = (t1 > t2) ? t1 : t2;

                                         QVector3D hitPoint = sp + raysDirection * t;

                                         QVector3D v1 = sp - hitPoint;
                                         QVector3D v2 = eyeCenter - hitPoint;

                                         double len1 = v1.length();
                                         double len2 = v2.length();

                                         if (len1 < 1e-9 || len2 < 1e-9)
                                             return std::nullopt;

                                         double cosTheta = QVector3D::dotProduct(v1, v2) / (len1 * len2);

                                         QVector3D color;

                                         if (cosTheta >= ca780 && cosTheta <= ca380)
                                             color = getColorFromCosAngle(lut1, cosTheta,1);
                                         else if (cosTheta >= cb380 && cosTheta <= cb780)
                                             color = getColorFromCosAngle(lut2, cosTheta,2);
                                         else
                                             return std::nullopt;

                                         return Vertex(hitPoint, color);
                                     });

    rainbowPoints.clear();
    rainbowPoints.reserve(results.size());

    for (int i = 0; i < results.size(); ++i)
    {
        const std::optional<Vertex> &v = results[i];

        if (v.has_value())
            rainbowPoints.push_back(v.value());
    }
}

void SceneX::drawWaterSpherePatch()
{
    float R = 101.0f;
    QVector3D center(-100,0,0);

    int stacks = 30;
    int slices = 60;

    // ограничиваем кусок сферы (не вся!)
    float phiMin = -0.0f;  // вниз
    float phiMax =  1.0f;  // вверх

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
    drawSurface();
    drawSphere(sunCenter, SunRadius, SunColor, 50, 50);

    for (int i = 0; i < rainbowPoints.size(); i++) {
        if (showBeams)
            drawRay(sunPoints[i], rainbowPoints[i].position, SunColor);

//drawSphere(rainbowPoints[i].position, 0.5f, QVector3D(rainbowPoints[i].color.x(),rainbowPoints[i].color.y(),rainbowPoints[i].color.z()), 6, 8);

        glPointSize(5.0f);
        glEnable(GL_POINT_SMOOTH);
        glColor3ub(rainbowPoints[i].color.x(),rainbowPoints[i].color.y(),rainbowPoints[i].color.z());

        glBegin(GL_POINTS);
        glVertex3d(rainbowPoints[i].position.x(),
                       rainbowPoints[i].position.y(),
                       rainbowPoints[i].position.z());
        glEnd();

        if (showBeams)
            drawRay(rainbowPoints[i].position, eyeCenter, QVector3D(rainbowPoints[i].color.x(),rainbowPoints[i].color.y(),rainbowPoints[i].color.z()));
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
