/* _____________________________________________
| |
| Main.cpp |
| Part of Mine DS , by CoolAs and Ray |
| Thanks to Dirbaio! |
|_____________________________________________|
 */

#include <nds.h>
#include <stdio.h>
#include <time.h>
#include "blocks.h"
#include "world.h"
#include "mobs.h"
#include "particles.h"
#include "fontHandler.h"
#include "worldgen.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "worldRender.h"
#include "titlescreen.h"
#include "mainGame.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "crafting.h"
#include "nifi.h"
#include "sounds.h"
#include "files.h"
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include <string>


int screen_x, screen_y;
void beginRender(int a, int b);
void renderWorld(worldObject* world);

int main()
{
	defaultExceptionHandler();
	srand(time(NULL));
	subBgInit();
	drawBackground();
	setupFont();
	nifiInit();
	mmStop();
	swiWaitForVBlank();
	initBlockProperties();
	worldRender_Init();
	graphicsInit();
	mobHandlerInit();
	worldRender_LoadSprites();
	craftingInit();
	graphicFrame();
	//videoSetMode(MODE_0_2D);
	//videoSetModeSub(MODE_0_2D);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	//vramSetBankD(VRAM_D_SUB_SPRITE);
	//oamInit(&oamMain, SpriteMapping_1D_128, false);
	//oamInit(&oamSub, SpriteMapping_1D_128, false);

	oamClear(&oamSub, 0, 127);
	while (1)
	{
		titlescreen();
	}
	return 1;
}
