/* _____________________________________________
| |
| Main.cpp |
| Part of Mine DS , by CoolAs and Ray |
| Thanks to Dirbaio! |
|_____________________________________________|
 */

#include <nds.h>
#include <stdio.h>
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
#include "general.h"
#include "blockupdate.h"
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include <string>


int screen_x, screen_y;
void beginRender(int a, int b);
void renderWorld(worldObject* world);

int main()
{
	defaultExceptionHandler();
	subBgInit();
	drawBackground();
	setupFont();
	nifiInit();
	swiWaitForVBlank();
	initBlockProperties();
	worldRender_Init();
	graphicsInit();
	mobHandlerInit();
	worldRender_LoadSprites();
	craftingInit();
  proceduralBlockUpdateInit();
	graphicFrame();
	oamClear(&oamSub, 0, 127);
	initFile();
	initSound();
	loadControls(getGlobalSettings()); //Silently load controls (May fail silently))
	clear_messages();

	/*FILE *inventoryFile;
	if ((inventoryFile = fopen(INVENTORY_PATH, "r")) != NULL)
	{
		for (int i = 0; i < NUM_INV_SPACES; ++i)
		{
			int l, id, quantity;
			fscanf(inventoryFile, "%d %d %d ", &l, &id, &quantity);
			iprintf("\x1b[8;1HIndex:%d\nID:%d Quantity:%d", l, id, quantity);
		}
	}
	else
		printXY(1, 1, "FAILED");
	printXY(1, 11, "DONE");
	sleep(100);*/
	titlescreen();
}
