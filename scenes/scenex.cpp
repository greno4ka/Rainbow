#include "scenex.h"


Vec3 normalize(Vec3 v)
{
    double len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return { v.x/len, v.y/len, v.z/len };
}


SceneX::SceneX()
{
    X = 800;
    Y = 600;

    sunCenter = { -100.0, 0.0, 50.0 };
    wallCenter = { 0.0, 0.0, 6.0 };

    raysDirection = {
        wallCenter.x - sunCenter.x,
        wallCenter.y - sunCenter.y,
        wallCenter.z - sunCenter.z
    };

    raysDirection = normalize(raysDirection);

    for (int i = 0; i < 100; i++)
    {
        raysAngle.push_back((double)rand() / RAND_MAX * 2.0 * M_PI);
        raysRadius.push_back(sqrt((double)rand() / RAND_MAX) * 10.0);
    }

    timer.start();
}


void drawSun(double cx, double cy, double cz, double radius)
{
    const int segments = 50;

    glColor3f(1.0f, 0.9f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);

    // центр
    glVertex3d(cx, cy, cz);

    for (int i = 0; i <= segments; i++)
    {
        double angle = 2.0 * M_PI * i / segments;
        double y = cy + cos(angle) * radius;
        double z = cz + sin(angle) * radius;

        glVertex3d(cx, y, z);
    }

    glEnd();
}

void SceneX::drawWall()
{
    double wallH = 10.0;
    double wallW = 6.0;

    glColor3f(0.7f, 0.7f, 1.0f);

    double x = wallCenter.x;
    double y0 = wallCenter.y - wallH;
    double y1 = wallCenter.y + wallH;
    double z0 = wallCenter.z - wallW;
    double z1 = wallCenter.z + wallW;

    glBegin(GL_TRIANGLE_FAN);

    glVertex3d(x, wallCenter.y, wallCenter.z);

    glVertex3d(x, y0, z0);
    glVertex3d(x, y0, z1);
    glVertex3d(x, y1, z1);
    glVertex3d(x, y1, z0);
    glVertex3d(x, y0, z0);

    glEnd();
}

void SceneX::drawSingleRay(double a, double r, double timeSec)
{
    // старт луча (точка на "солнце")
    Vec3 p = {
        sunCenter.x,
        std::cos(a) * r,
        sunCenter.z + std::sin(a) * r
    };

    double kSpeed=50;
    double t = kSpeed * std::fmod(timeSec, 10.0);

    // точка без ограничений (если бы стены не было)
    Vec3 q_free = {
        p.x + raysDirection.x * t,
        p.y + raysDirection.y * t,
        p.z + raysDirection.z * t
    };

    Vec3 q = q_free;

    // пересечение со стеной (по X)
    if (std::abs(raysDirection.x) > 1e-9)
    {
        double t_hit = (wallCenter.x - p.x) / raysDirection.x;

        if (t_hit >= 0.0 && t_hit < t)
        {
            q = {
                p.x + raysDirection.x * t_hit,
                p.y + raysDirection.y * t_hit,
                p.z + raysDirection.z * t_hit
            };
        }
    }

    glColor3f(1.0f, 1.0f, 0.2f);
    glBegin(GL_LINES);
    glVertex3d(p.x, p.y, p.z);
    glVertex3d(q.x, q.y, q.z);
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

    drawSun(-100.0, 0.0, 50.0, 10.0);

    for (int i = 0; i < 100; i++)
        drawSingleRay(raysAngle[i], raysRadius[i], timeSec);
}

void SceneX::updateXY(int newX, int newY)
{
    X = newX;
    Y = newY;
}