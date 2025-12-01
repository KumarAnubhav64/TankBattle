#include "Game.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./tank_battle <mode> [ip]" << std::endl;
        std::cout << "Modes: local, server, client" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string ip = "";
    if (argc > 2) ip = argv[2];

    Game game;
    game.init(mode, ip);
    game.run();

    return 0;
}
