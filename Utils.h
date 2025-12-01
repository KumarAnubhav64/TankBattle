#ifndef UTILS_H
#define UTILS_H

#include <cmath>

struct Vector2 {
    float x, y;
};

// Simple packet to sync tank state
struct TankState {
    float x, y;
    float angle;
    bool isShooting;
    int health;
    int score1;
    int score2;
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PORT = 12345;
const float PI = 3.1415926535f;

#endif
