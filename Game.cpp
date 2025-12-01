#include "Game.h"
#include <iostream>
#include <graphics.h>

Game::Game() {
    isRunning = false;
    isLocalMode = false;
    isServer = false;
    myTank = nullptr;
    oppTank = nullptr;
    score1 = 0;
    score2 = 0;
}

Game::~Game() {
    if (myTank) delete myTank;
    if (oppTank) delete oppTank;
    closegraph();
}

void Game::init(const std::string& mode, const std::string& ip) {
    if (mode == "local") {
        isLocalMode = true;
        myTank = new Tank(0, 100, 240, GREEN);
        oppTank = new Tank(1, 540, 240, RED);
        oppTank->angle = 180;
    } else if (mode == "server") {
        isServer = true;
        if (!network.initServer()) {
            std::cerr << "Failed to start server" << std::endl;
            exit(1);
        }
        myTank = new Tank(0, 100, 240, GREEN);
        oppTank = new Tank(1, 540, 240, RED);
        oppTank->angle = 180;
    } else if (mode == "client") {
        isServer = false;
        if (!network.initClient(ip)) {
            std::cerr << "Failed to connect to " << ip << std::endl;
            exit(1);
        }
        // Client is Player 2 (Red)
        myTank = new Tank(1, 540, 240, RED);
        myTank->angle = 180;
        oppTank = new Tank(0, 100, 240, GREEN);
    }

    // int gd = DETECT, gm;
    // initgraph(&gd, &gm, NULL);
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    bullets.resize(20);
    isRunning = true;
}

#include <unistd.h>

void Game::run() {
    while (isRunning) {
        handleInput();
        update();
        render();
        usleep(30000);
    }
}

void Game::handleInput() {
    bool p1_up=0, p1_down=0, p1_left=0, p1_right=0, p1_shoot=0;
    bool p2_up=0, p2_down=0, p2_left=0, p2_right=0, p2_shoot=0;

    if (kbhit()) {
        int key = getch();
        if (key == 27) isRunning = false;

        // P1 Controls (WASD + Space)
        if (key == 'w') p1_up = true;
        if (key == 's') p1_down = true;
        if (key == 'a') p1_left = true;
        if (key == 'd') p1_right = true;
        if (key == ' ') p1_shoot = true;

        // P2 Controls (IJKL + Enter)
        if (key == 'i') p2_up = true;
        if (key == 'k') p2_down = true;
        if (key == 'j') p2_left = true;
        if (key == 'l') p2_right = true;
        if (key == 13) p2_shoot = true;
    }

    if (isLocalMode) {
        myTank->update(p1_up, p1_down, p1_left, p1_right, p1_shoot, bullets);
        oppTank->update(p2_up, p2_down, p2_left, p2_right, p2_shoot, bullets);
    } else {
        // Network Mode: My Tank uses P1 controls (WASD)
        myTank->update(p1_up, p1_down, p1_left, p1_right, p1_shoot, bullets);
    }
}

void Game::update() {
    for (auto& b : bullets) b.update();
    checkCollisions();

    if (!isLocalMode) syncNetwork();
}

void Game::syncNetwork() {
    TankState myState = myTank->getState();
    myState.score1 = score1; // Send my view of scores
    myState.score2 = score2;
    network.sendTankState(myState);

    TankState oppState;
    if (network.receiveTankState(oppState)) {
        oppTank->setState(oppState);
        // Sync scores (Server is authority usually, but simple sync here)
        if (!isServer) {
             score1 = oppState.score1;
             score2 = oppState.score2;
        }
    }
}

void Game::checkCollisions() {
    for (auto& b : bullets) {
        if (b.active) {
            if (myTank->checkCollision(b)) {
                if (myTank->health <= 0) {
                    score2++;
                    resetRound();
                }
            }
            if (oppTank->checkCollision(b)) {
                if (oppTank->health <= 0) {
                    score1++;
                    resetRound();
                }
            }
            if (b.isOffScreen()) b.active = false;
        }
    }
}

void Game::resetRound() {
    // Reset positions and health
    myTank->health = 3;
    oppTank->health = 3;
    bullets.clear();
    bullets.resize(20);

    if (isLocalMode || isServer) {
        // Host/Local controls positions
        myTank->x = 100; myTank->y = 240; myTank->angle = 0;
        oppTank->x = 540; oppTank->y = 240; oppTank->angle = 180;
    } else {
        // Client just resets local state, server will sync positions
        myTank->x = 540; myTank->y = 240; myTank->angle = 180;
        oppTank->x = 100; oppTank->y = 240; oppTank->angle = 0;
    }

    usleep(1000000); // Pause before next round
}

void Game::drawUI() {
    // Top Bar
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, 0, SCREEN_WIDTH, 30);

    setcolor(WHITE);
    setbkcolor(DARKGRAY);

    char msg[50];
    sprintf(msg, "P1 (Green): %d   vs   P2 (Red): %d", score1, score2);
    outtextxy(SCREEN_WIDTH/2 - 100, 8, msg);
    setbkcolor(BLACK); // Reset
}

void Game::render() {
    cleardevice();

    // Grid Background
    setcolor(COLOR(20, 20, 20));
    for(int i=0; i<SCREEN_WIDTH; i+=40) line(i, 0, i, SCREEN_HEIGHT);
    for(int i=0; i<SCREEN_HEIGHT; i+=40) line(0, i, SCREEN_WIDTH, i);

    myTank->draw();
    oppTank->draw();
    for (auto& b : bullets) b.draw();

    drawUI();
}
