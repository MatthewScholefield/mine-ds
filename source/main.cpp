#include <nds.h>
#include "FileSystem.hpp"
#include "graphics/Graphic.hpp"
#include "utils.hpp"
#include "graphics/Graphics.hpp"
#include "graphics/Font.hpp"
#include "graphics/SubRenderer.hpp"
#include "SoundSystem.hpp"
#include "MessageSystem.hpp"
#include "graphics/MainRenderer.hpp"
#include "World.hpp"
#include "Player.hpp"
#include <memory>


int main() {
    defaultExceptionHandler();
    initFile();

    Graphics graphics;
    SubRenderer subRenderer(graphics);
    MainRenderer mainRenderer(graphics);

    SoundSystem soundSystem;
    MessageSystem messageSystem;

    std::unique_ptr<World> world(new World());
    world->generate();

    soundSystem.playMusic(Music::Hal2);

    Player player(graphics);
    int count = 0;

    while (true) {
        scanKeys();
        messageSystem.update();
        soundSystem.update();
        world->update();
        printXY(5, 5, "Hi %d", ++count);
        player.update(*world, 1 / 60.f);
        mainRenderer.updateCenter(player.getPos() * float(World::blockSize));
        swiWaitForVBlank();
        {
            world->render(mainRenderer);
            player.render(mainRenderer);
            mainRenderer.update();
            subRenderer.update();
        }
    }
}
