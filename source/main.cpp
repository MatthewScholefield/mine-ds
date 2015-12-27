#include <nds.h>
#include <stdio.h>
#include "blocks.h"
#include "world/WorldObject.h"
#include "mobs.h"
#include "particles.h"
#include "fontHandler.h"
#include "world/worldGen.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "world/worldRender.h"
#include "titleScreen.h"
#include "mainGame.h"
#include "graphics/graphics.h"
#include "graphics/particleHandler.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "sounds.h"
#include "files.h"
#include "general.h"
#include "blockUpdaters/blockUpdater.h"
//#include <maxmod9.h>    // Maxmod definitions for ARM9
//#include <string>
#include "graphics/3DHandler.h"
#include "localizations/locale.h"


int screen_x, screen_y;
void beginRender(int a, int b);
void renderWorld(WorldObject &world);

int main()
{
	defaultExceptionHandler();
	initFile();
	initBlockProperties();
	loadConfig(getGlobalSettings()); //May fail silently
	subBgInit();
	setupFont();
	graphicsInit();
	irqSet(IRQ_HBLANK, gradientHandler);
	irqEnable(IRQ_HBLANK);
	mobHandlerInit();
	proceduralBlockUpdateInit();
	initSound();
	clear_messages();
	init3D();

	setLanguage(1);
	titleScreen();

	return 0;
}
