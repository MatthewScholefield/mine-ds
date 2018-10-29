#pragma once

#include <nds.h>

// This function sets up the font and console used for printf("Hello World\n");.
class TitleFontSystem {
public:
    TitleFontSystem();
    int getConsoleId();
    void clearTextRegion(int x, int y, int sx, int sy);
    void clearText(bool firstSection, int sizeY);
    void clearText();
    void refresh();

private:
    static bool putChar(void *con, char c);
    ConsoleFont font;
    PrintConsole *console;
};
