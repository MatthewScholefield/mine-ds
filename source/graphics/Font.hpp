#pragma once

#include <nds.h>

// This function sets up the font and console used for printf("Hello World\n");.
class Font {
public:
    // Number of chars in x and y direction
    static constexpr int cx = 32, cy = 12;  // Unverified

    Font();
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
