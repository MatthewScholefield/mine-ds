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
#include "deathScreen.h"
bool up;
Graphic graphics[10];
void mobHandlerUpdate(worldObject* world);
void mainGame(int Mode)
{
	mobsReset();
	consoleClear();
	lcdMainOnBottom();
	iprintf("\x1b[7;0HGenerating world...\n");
	touchPosition touch;
	worldObject* CurrentWorld;
	int i,j;
	if (Mode==0) // Generate new world!
	{
		CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
		generateWorld(CurrentWorld);
		Calculate_Brightness(CurrentWorld);
	}	
	CurrentWorld->CamX=0;
	CurrentWorld->CamY=0;
	//loadGraphicSub(&graphics[9],2,8);
	consoleClear();	
	while(1){
		scanKeys();
		touchRead(&touch);
		miningUpdate(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY,touch,keysDown());
		update_message();
		if (keysDown() & KEY_START) break;
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
		mobHandlerUpdate(CurrentWorld);
		if (deathScreenUpdate()) break;
		worldRender_Render(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY);
	}
	free(CurrentWorld);
}
