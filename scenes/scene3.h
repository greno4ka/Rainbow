#ifndef SCENE3_H
#define SCENE3_H

#include <QOpenGLFunctions>

#include "scenebase.h"
#include "beam.h"

class Scene3 : public SceneBase
{
    int displayMode;
    bool dynamicMode;
    int desiredFPS;

    // used for floor and man
    const double ManPositionX = -10,
                 ManPositionY = -11,
                 ManHeight = 3; // To be honest, it's height of his eye

    const double CloudWidth = 3,
                 CloudHeight = 1,
                 CloudRadius = 6,
                 CloudCenterX = 15,
                 CloudCenterY = 15;

    const int RainStep = 20,    // pixels between two rain dashes
              RainKoef = 5;     // simply k in y=kx

    const int NumberOfBeams = 100;

    double *sunlightPenetration;

    int cloudBegin, cloudEnd,   // x-bounds of cloud
        numberOfRainDashes;     // number of elements of currentRainSpeed

    int *currentRainSpeed,
        *initialRainSwift,
        *currentRainFrame;

public:
    Scene3();
    ~Scene3();

    void setDisplayMode(int newDisplayMode);
    void setDesiredFPS(int newDesiredFPS);
    void switchDynamicMode();

    void regenerateRain();


    void drawCloud();
    void drawRain();
    void drawFloor();
    void drawMan();

    void display();

    bool getDynamicMode() const;
};

#endif // SCENE3_H
