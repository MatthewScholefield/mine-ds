#include <nds.h>
#include "startscreen.h"
startscreen::startscreen(){
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	//dmaCopy(startscreenbgBitmap, bgGetGfxPtr(bg3), 256*256);
	//dmaCopy(startscreenbgPal, BG_PALETTE, 256*2);
}
void startscreen:showscreen(){
//xD
}