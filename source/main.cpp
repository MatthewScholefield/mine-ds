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

    Graphics graphicsSystem;

    Font font;
    SubRenderer subRenderer(font, graphicsSystem);
    MainRenderer mainRenderer(graphicsSystem);

    graphicsSystem.bind(subRenderer);
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
            graphicsSystem.beginRender(0, 0);
            world->render(mainRenderer);
            oamUpdate(&oamMain);
            subRenderer.updateSubBG();
        }
    }
}
