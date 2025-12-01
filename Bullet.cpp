#include "Bullet.h"
#include <cmath>

Bullet::Bullet() {
    active = false;
    speed = 10.0f;
    ownerId = -1;
}

void Bullet::spawn(float startX, float startY, float startAngle, int owner) {
    x = startX;
    y = startY;
    angle = startAngle;
    active = true;
    ownerId = owner;
}

void Bullet::update() {
    if (!active) return;
    // Standard Trig: Y is Up. Screen: Y is Down.
    // We use Y-Up logic for angle (CCW), so we subtract sin from Y.
    float rad = angle * PI / 180.0f;
    x += cos(rad) * speed;
    y -= sin(rad) * speed;
}

void Bullet::draw() {
    if (!active) return;
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse((int)x, (int)y, 3, 3);
}

bool Bullet::isOffScreen() {
    return x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT;
}
