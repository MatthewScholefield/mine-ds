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

    lcdMainOnBottom();
    subRenderer.drawBackground();
    subRenderer.setSubBg(0, 0);
    
    world->generate();

    soundSystem.playMusic(Music::Hal2);


    while (true) {
        static int val = 0;
        if (val % 100 == 0) {
            std::string s = "Main: " + std::to_string(rand() % 100);
            messageSystem.print(s);
        }

        messageSystem.update();
        soundSystem.update();
        timeUpdate();
        world->update();
        swiWaitForVBlank();
        {
            printXY(1, 1, "HELLO! %d", ++val);
            graphicsSystem.beginRender(0, 0);
            for (int i = 0; i < 10; ++i) {
                world->render(mainRenderer);
            }
            oamUpdate(&oamMain);
            subRenderer.updateSubBG();
        }
    }
}
