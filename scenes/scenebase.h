#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <QOpenGLFunctions>
#include <QObject>

#include <cmath>

class SceneBase : public QObject
{
    Q_OBJECT
signals:
    void requestUpdate();

protected:
    static const int ImageQuality = 100;
    static const int DropRadius = 5;

    int X, Y; // Window height and width

    int SceneScale;
    int offsetXFactor,
        offsetYFactor,

        scaleXFactor,
        scaleYFactor;

    /// Methods of recalculation decart coords to screen coords
    inline double x(double x0)
    {
        return X - (X/offsetXFactor) + x0 * std::min(X/scaleXFactor, Y/scaleYFactor) / SceneScale;
    }

    inline double y(double y0)
    {
        return Y - (Y/offsetYFactor) + y0 * std::min(X/scaleXFactor, Y/scaleYFactor) / SceneScale;
    }

    inline double r(double r0)
    {
        return r0 * std::min(X/scaleXFactor, Y/scaleYFactor) / SceneScale;
    }

    void drawLine(double x0, double y0, double x1, double y1);

    void draw_drop();
    void draw_axes();

public:
    SceneBase(QObject *parent = nullptr);

    void updateXY(int newX, int newY);
    virtual void display() = 0;
};

#endif // SCENEBASE_H
