#ifndef BULLET_H
#define BULLET_H

#include "Utils.h"
#include <graphics.h>

class Bullet {
public:
    float x, y;
    float angle;
    bool active;
    float speed;
    int ownerId; // 0 for P1, 1 for P2

    Bullet();
    void spawn(float startX, float startY, float startAngle, int owner);
    void update();
    void draw();
    bool isOffScreen();
};

#endif
