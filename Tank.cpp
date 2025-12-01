#include "Tank.h"
#include <cmath>
#include <cstdio>

Tank::Tank(int id, float startX, float startY, int tankColor) {
    this->id = id;
    x = startX;
    y = startY;
    angle = 0;
    color = tankColor;
    health = 3;
    cooldown = 0;
}

void Tank::update(bool up, bool down, bool left, bool right, bool shoot, std::vector<Bullet>& bullets) {
    if (health <= 0) return;

    if (left) rotate(5.0f);
    if (right) rotate(-5.0f);

    if (up) move(3.0f);
    if (down) move(-3.0f);

    if (cooldown > 0) cooldown--;

    if (shoot && cooldown == 0) {
        for (auto& b : bullets) {
            if (!b.active) {
                float rad = angle * PI / 180.0f;
                float tipX = x + cos(rad) * 25;
                float tipY = y - sin(rad) * 25;
                b.spawn(tipX, tipY, angle, id);
                cooldown = 20; // 20 frames cooldown
                break;
            }
        }
    }
}

void Tank::move(float step) {
    float rad = angle * PI / 180.0f;
    x += cos(rad) * step;
    y -= sin(rad) * step;

    if (x < 15) x = 15;
    if (x > SCREEN_WIDTH - 15) x = SCREEN_WIDTH - 15;
    if (y < 15) y = 15;
    if (y > SCREEN_HEIGHT - 15) y = SCREEN_HEIGHT - 15;
}

void Tank::rotate(float step) {
    angle += step;
    if (angle >= 360) angle -= 360;
    if (angle < 0) angle += 360;
}

void Tank::draw() {
    if (health <= 0) return;

    setcolor(color);
    setfillstyle(SOLID_FILL, color);

    // Tank Body (Rectangular with rotation)
    // We need to calculate 4 corners based on angle
    float rad = angle * PI / 180.0f;
    float c = cos(rad);
    float s = sin(rad);

    // Dimensions
    float w = 30; // Half width
    float h = 20; // Half height

    // Corners relative to center (x,y)
    // FL, FR, BR, BL
    int p[8];
    p[0] = x + (w * c - h * s); p[1] = y - (w * s + h * c);
    p[2] = x + (w * c + h * s); p[3] = y - (w * s - h * c);
    p[4] = x + (-w * c + h * s); p[5] = y - (-w * s - h * c);
    p[6] = x + (-w * c - h * s); p[7] = y - (-w * s + h * c);

    fillpoly(4, p);

    // Tracks (Darker lines on sides)
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, 3);
    line(p[0], p[1], p[6], p[7]); // Left/Top track
    line(p[2], p[3], p[4], p[5]); // Right/Bottom track
    setlinestyle(SOLID_LINE, 0, 1);

    // Turret (Circle)
    setcolor(color);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse((int)x, (int)y, 10, 10);

    // Barrel
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, 3);
    int x2 = (int)(x + c * 35);
    int y2 = (int)(y - s * 35);
    line((int)x, (int)y, x2, y2);
    setlinestyle(SOLID_LINE, 0, 1);

    // Health Bar above tank
    int barW = 40;
    int barH = 4;
    int barX = x - barW/2;
    int barY = y - 40;

    setcolor(WHITE);
    rectangle(barX, barY, barX + barW, barY + barH);

    if (health > 0) {
        setfillstyle(SOLID_FILL, (health > 1 ? GREEN : RED));
        int fillW = (health * barW) / 3;
        bar(barX + 1, barY + 1, barX + fillW, barY + barH);
    }
}

void Tank::setState(TankState state) {
    x = state.x;
    y = state.y;
    angle = state.angle;
    health = state.health;
}

TankState Tank::getState() {
    TankState s;
    s.x = x;
    s.y = y;
    s.angle = angle;
    s.health = health;
    s.isShooting = false;
    return s;
}

bool Tank::checkCollision(Bullet& b) {
    if (!b.active || b.ownerId == id || health <= 0) return false;

    float dx = x - b.x;
    float dy = y - b.y;
    float dist = sqrt(dx*dx + dy*dy);

    if (dist < 15 + 3) {
        b.active = false;
        health--;
        return true;
    }
    return false;
}
