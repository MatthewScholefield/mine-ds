#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldgen.h"
#include "message.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "worldRender.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "mobs/mobPlayer.h"
#include "deathScreen.h"
#include "daynight.h"
#include "graphics/inventoryGraphics.h"

bool up;
Graphic graphics[10];
void mobHandlerUpdate(worldObject* world);

worldObject* mainGame(int mode,worldObject* CurrentWorld)
{
	consoleClear();
	clear_messages();
	lcdMainOnBottom();
	touchPosition touch;
	initInvGraphics();
	initHardness();

	if (mode==0) // Generate new world and reset the camera and time
	{
		iprintf("\x1b[8;0HGenerating world...\n");
		if (CurrentWorld==NULL) CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
		if (CurrentWorld==NULL)
		{
			return NULL;
		}
		mobsReset();
		generateWorld(CurrentWorld);
		iprintf("\x1b[8;0HA");
		CurrentWorld->CamX=0;
		CurrentWorld->CamY=0;
		CurrentWorld->timeInWorld=0;
		consoleClear();
	}
	while(1){ //Infinitely repeats until break
		scanKeys();
		touchRead(&touch);
		miningUpdate(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY,touch,keysDown());
		mobHandlerUpdate(CurrentWorld);
		update_message();
		if (keysDown() & KEY_START || shouldQuitGame())
			break;
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
		timeUpdate(CurrentWorld);
		worldRender_Render(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY);
	}
	return CurrentWorld;
}
