#include <font.h>
#include <nds/arm9/background.h>
#include "TitleFontSystem.hpp"

TitleFontSystem::TitleFontSystem() {
    const int tileBase = 1;
    const int mapBase = 0;
    console = consoleInit(nullptr, 0, BgType_Text8bpp, BgSize_T_512x512, mapBase, tileBase, false, false);
    consoleSetWindow(console, 0, 0, 64, 64);
    console->PrintChar = putChar;
    font.gfx = (u16 *) fontTiles;
    font.pal = (u16 *) fontPal;
    font.numChars = 256;
    font.numColors = fontPalLen / 2;
    font.bpp = 8;
    font.asciiOffset = 0;
    font.convertSingleColor = false;
    consoleSetFont(console, &font);
}

bool TitleFontSystem::putChar(void *con, char c) {
    auto *currentConsole = (PrintConsole *) con;

    if (currentConsole->cursorX >= currentConsole->windowWidth) {
        currentConsole->cursorX = 0;
        ++currentConsole->cursorY;
    }
    if (c == '\n') {
        currentConsole->cursorX = 0;
        ++currentConsole->cursorY;
        return true;
    }

    int offset = 0;
    if (currentConsole->cursorX < 32) {
        offset += currentConsole->cursorX;
    }
    else
        offset += (currentConsole->cursorX - 32) + 32 * 32;

    if (currentConsole->cursorY < 32) {
        offset += currentConsole->cursorY * 32;
    }
    else
        offset += (currentConsole->cursorY - 32) * 32 + 32 * 64;

    currentConsole->fontBgMap[offset] =
            currentConsole->fontCurPal | (u16) (c + currentConsole->fontCharOffset - currentConsole->font.asciiOffset);
    ++currentConsole->cursorX;
    return true;
}

int TitleFontSystem::getConsoleId() {
    return console->bgId;
}

void TitleFontSystem::clearTextRegion(int x, int y, int sx, int sy) {
    u16 *map = console->fontBgMap;
    for (int i = x; i < x + sx; ++i) {
        for (int j = y; j < y + sy; ++j) {
            int offset = 0;
            if (i < 32) {
                offset += i;
            }
            else
                offset += (i - 32) + 32 * 32;

            if (j < 32) {
                offset += j * 32;
            }
            else
                offset += (j - 32) * 32 + 64 * 32;
            map[offset] = 0;
        }
    }
}

void TitleFontSystem::refresh() {
    consoleSetFont(nullptr, &font); //The nullptr parameter is understood as the current console
}

void TitleFontSystem::clearText(bool firstSection, int sizeY) {
    clearTextRegion(firstSection ? 0 : 32, 0, 32, sizeY);
}

void TitleFontSystem::clearText() {
    clearTextRegion(0, 0, 32, 24);
}
