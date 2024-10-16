//
// Created by Giovanni Sgaravatto
//

#include "Game.h"

int main(int argc, char* argv[]) {
    try {
        Game monopoly(argc, argv);
        monopoly.startGame();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
