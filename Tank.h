#ifndef TANK_H
#define TANK_H

#include "Utils.h"
#include "Bullet.h"
#include <vector>
#include <graphics.h>

class Tank {
public:
    float x, y;
    float angle;
    int health;
    int color;
    int cooldown;
    int id;

    Tank(int id, float startX, float startY, int tankColor);
    void update(bool up, bool down, bool left, bool right, bool shoot, std::vector<Bullet>& bullets);
    void draw();
    void setState(TankState state);
    TankState getState();
    bool checkCollision(Bullet& b);

private:
    void move(float step);
    void rotate(float step);
};

#endif
