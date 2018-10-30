#include <nds.h>
#include "FileSystem.hpp"
#include "graphics/Graphic.hpp"
#include "utils.hpp"
#include "graphics/GraphicsSystem.hpp"
#include "graphics/TitleFontSystem.hpp"
#include "graphics/SubRenderer.hpp"
#include "SoundSystem.hpp"
#include "MessageSystem.hpp"
#include "graphics/MainRenderer.hpp"


int main() {
    defaultExceptionHandler();

    initFile();

    GraphicsSystem graphicsSystem;
    TitleFontSystem titleFontSystem;
    SubRenderer titleGraphics(titleFontSystem, graphicsSystem);
    MainRenderer mainRenderer(graphicsSystem);

    graphicsSystem.bind(titleGraphics);
    SoundSystem soundSystem;
    MessageSystem messageSystem;

    lcdMainOnBottom();
    titleGraphics.drawBackground();
    titleGraphics.setSubBg(0, 0);

    soundSystem.playMusic(Music::Hal2);

    while (true) {
        static int val = 0;
        if (val % 100 == 0) {
            std::string s = "Main: " + std::to_string(rand() % 100);
            messageSystem.print(s);
        }

        messageSystem.update();
        soundSystem.update();
        swiWaitForVBlank();
        {
            printXY(1, 1, "HELLO! %d", ++val);
            graphicsSystem.beginRender(0, 0);
            for (int i = 0; i < 10; ++i) {
                mainRenderer.renderBlock(i, 0, 31);
            }
            oamUpdate(&oamMain);
            titleGraphics.updateSubBG();
        }
    }
}
