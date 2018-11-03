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

    while (true) {
        messageSystem.update();
        soundSystem.update();
        timeUpdate();
        world->update();
        swiWaitForVBlank();
        {
            world->render(mainRenderer);
            oamUpdate(&oamMain);
            subRenderer.update();
        }
    }
}
