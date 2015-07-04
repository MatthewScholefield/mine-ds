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
//#include "nifi.h"
#include "sounds.h"
#include "files.h"
#include "general.h"
#include "blockupdate.h"
//#include <maxmod9.h>    // Maxmod definitions for ARM9
//#include <string>


int screen_x, screen_y;
void beginRender(int a, int b);
void renderWorld(WorldObject* world);

int main()
{
	defaultExceptionHandler();
	initFile();
	//nifiInit();

	initBlockProperties();
	loadConfig(getGlobalSettings()); //Silently load controls (May fail silently))
	subBgInit();
	setupFont();
	graphicsInit();
	//irqInit();
	irqSet(IRQ_HBLANK, gradientHandler);
	irqEnable(IRQ_HBLANK);
	mobHandlerInit();
	worldRender_LoadSprites();
	craftingInit();
	proceduralBlockUpdateInit();
	graphicFrame();
	oamClear(&oamSub, 0, 127);
	initSound();
	clear_messages();
	titlescreen();
	return 0;
}
