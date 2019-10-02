#include <nds.h>
#include "FileSystem.hpp"
#include "Game.hpp"


int main() {
    defaultExceptionHandler();
    initFile();
    
    auto game = std::make_unique<Game>();

    while (true) {
        game->update();
    }
}
