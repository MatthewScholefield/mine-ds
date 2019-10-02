#include <nds.h>
#include "FileSystem.hpp"
#include "Game.hpp"


int main() {
    defaultExceptionHandler();
    initFile();
    
    Game game;

    while (true) {
        game.update();
    }
}
