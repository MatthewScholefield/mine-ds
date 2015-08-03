#include <nds.h>
#include <stdio.h>
#include "blocks.h"
#include "world.h"
#include "mobs.h"
#include "particles.h"
#include "fontHandler.h"
#include "worldGen.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "worldRender.h"
#include "titleScreen.h"
#include "mainGame.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "crafting.h"
//#include "nifi.h"
#include "sounds.h"
#include "files.h"
#include "general.h"
#include "blockUpdater.h"
//#include <maxmod9.h>    // Maxmod definitions for ARM9
//#include <string>

#include "graphics/3DHandler.h"


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
	irqSet(IRQ_HBLANK, gradientHandler);
	irqEnable(IRQ_HBLANK);
	mobHandlerInit();
	craftingInit();
	proceduralBlockUpdateInit();
	clearMainGraphics();
	oamClear(&oamSub, 0, 127);
	initSound();
	clear_messages();
	init3D();

	titleScreen();
	return 0;
}
