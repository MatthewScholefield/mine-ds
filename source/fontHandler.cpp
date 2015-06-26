#include <nds.h>
#include "font.h"
///@breif This function sets up the font and console used for iprintf("Hello World\n");.
///It took a while to get working with the subBgTiles, please don't modify unless you know what you are doing.
ConsoleFont font;
PrintConsole* console;
int consoleID;

void clearTextRegion(int x, int y, int sx, int sy)
{
	u16* map = console->fontBgMap;
	for (int i = x; i < x + sx; ++i)
		for (int j = y; j < y + sy; ++j)
		{
			int offset = 0;
			if (i < 32)
				offset += i;
			else
				offset += (i - 32) + 32 * 32;

			if (j < 32)
				offset += j * 32;
			else
				offset += (j - 32) * 32 + 64 * 32;
			map[offset] = 0;
		}
}

bool put_char(void* con, char c)
{
	PrintConsole* currentConsole = (PrintConsole*) con;

	if (currentConsole->cursorX >= currentConsole->windowWidth)
	{
		currentConsole->cursorX = 0;
		++currentConsole->cursorY;
	}

	int offset = currentConsole->cursorY * 32;
	if (currentConsole->cursorX < 32)
		offset += currentConsole->cursorX;
	else
		offset += (currentConsole->cursorX - 32) + 32 * 32;

	currentConsole->fontBgMap[offset] = currentConsole->fontCurPal | (u16) (c + currentConsole->fontCharOffset - currentConsole->font.asciiOffset);
	++currentConsole->cursorX;
	return true;
}

void setupFont()
{
	const int tile_base = 1;
	const int map_base = 0;
	console = consoleInit(0, 0, BgType_Text8bpp, BgSize_T_512x512, map_base, tile_base, false, false);
	consoleSetWindow(console, 0, 0, 64, 64);
	console->PrintChar = put_char;
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
	return console->bgId;
}

void refreshFont()
{
	consoleSetFont(NULL, &font); //The NULL parameter is understood as the current console
}