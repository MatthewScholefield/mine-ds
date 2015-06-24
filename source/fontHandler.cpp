#include <nds.h>
#include "font.h"
///@breif This function sets up the font and console used for iprintf("Hello World\n");.
///It took a while to get working with the subBgTiles, please don't modify unless you know what you are doing.
ConsoleFont font;
int consoleID;

void setupFont()
{
	const int tile_base = 1;
	const int map_base = 0;
	PrintConsole *console = consoleInit(0, 0, BgType_Text8bpp, BgSize_T_256x256, map_base, tile_base, false, false);
	consoleSetWindow(console, 0, 0, 31, 31);
	consoleID = console->bgId;
	font.gfx = (u16*) fontTiles;
	font.pal = (u16*) fontPal;
	font.numChars = 256;
	font.numColors = fontPalLen / 2;
	font.bpp = 8;
	font.asciiOffset = 0;
	font.convertSingleColor = false;
	consoleSetFont(console, &font);
}

int getConsoleID()
{
	return consoleID;
}

void refreshFont()
{
	consoleSetFont(NULL, &font); //The NULL parameter is understood as the current console
}