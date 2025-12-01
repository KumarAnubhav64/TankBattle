#ifndef GAME_H
#define GAME_H

#include "Tank.h"
#include "Network.h"
#include <vector>
#include <string>

class Game {
public:
    Game();
    ~Game();
    void init(const std::string& mode, const std::string& ip = "");
    void run();

private:
    bool isRunning;
    bool isLocalMode;
    bool isServer;

    Tank* myTank;
    Tank* oppTank;

    Network network;
    std::vector<Bullet> bullets;

    int score1;
    int score2;

    void handleInput();
    void update();
    void render();
    void drawUI();
    void syncNetwork();
    void checkCollisions();
    void resetRound();
};

#endif
