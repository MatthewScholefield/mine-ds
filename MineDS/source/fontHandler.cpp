#include <nds.h>
#include <stdio.h>
#include "font.h"
///@breif This function sets up the font and console used for iprintf("Hello World\n");.
///It took a while to get working with the subBgTiles, please don't modify unless you know what you are doing.
void setupFont()
{
	const int tile_base = 1;
	const int map_base = 0;
	
	

	PrintConsole *console = consoleInit(0,0, BgType_Text8bpp, BgSize_T_256x256, map_base, tile_base, false, false);
	ConsoleFont font;

	font.gfx = (u16*)fontTiles;
	font.pal = (u16*)fontPal;
	font.numChars = 256;
	font.numColors =  fontPalLen / 2;
	font.bpp = 8;
	font.asciiOffset = 0;
	font.convertSingleColor = false;
	consoleSetFont(console, &font);
	iprintf("\x1b[7;0H");
	iprintf("Font setup complete!\n");
}
