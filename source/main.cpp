#include <nds.h>
#include "FileSystem.hpp"
#include "Graphic.hpp"
#include "subBgHandler.hpp"
#include "utils.hpp"
#include "GraphicsSystem.hpp"
#include "TitleFontSystem.hpp"
#include "TitleGraphicsSystem.hpp"
#include "SoundSystem.hpp"
#include "MessageSystem.hpp"


int main() {
    defaultExceptionHandler();

    initFile();

    GraphicsSystem graphicsSystem;
    TitleFontSystem titleFontSystem;
    TitleGraphicsSystem titleGraphics(titleFontSystem, graphicsSystem);

    graphicsSystem.bind(titleGraphics);
    SoundSystem soundSystem;
    MessageSystem messageSystem;

    lcdMainOnBottom();
    titleGraphics.drawBackground();
    titleGraphics.setSubBg(0, 0);

    soundSystem.playMusic(Music::MUSIC_HAL2);

    while (true) {
        static int val = 0;
        messageSystem.update();
        if (val % 100 == 0) {
            std::string s = "Main: " + std::to_string(rand() % 100);
            messageSystem.print(s);
        }
        if (soundSystem.streamIsOpen())
            mmStreamUpdate();
        swiWaitForVBlank();
        {
            printXY(1, 1, "HELLO! %d", ++val);
            graphicsSystem.beginRender(0, 0);
            for (int i = 0; i < 10; ++i) {
                graphicsSystem.renderBlock(i, 0, 31);
            }
            oamUpdate(&oamMain);
            titleGraphics.updateSubBG();
        }
    }
}
