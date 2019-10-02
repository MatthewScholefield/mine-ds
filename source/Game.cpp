#include <ctime>
#include "Game.hpp"


Game::Game()
        : graphics(), subRenderer(graphics), mainRenderer(graphics),
        soundSystem(), messageSystem(), world(std::make_unique<World>()),
        player(graphics, mainRenderer.getCam()) {
    srand((unsigned int) time(nullptr));
    world->generate();
    
    soundSystem.playMusic(Music::Hal2);
    player.respawn(*world, mainRenderer);
}


void Game::update() {
    scanKeys();
    messageSystem.update();
    soundSystem.update();
    world->update();
    player.update(*world, 1 / 60.f);
    mainRenderer.updateCenter(player.getPos());
    swiWaitForVBlank();
    {
        mainRenderer.begin();
        world->render(mainRenderer);
        player.render(mainRenderer);
        mainRenderer.update();
        subRenderer.update();
    }
}
