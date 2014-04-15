#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldgen.h"
#include "message.h"
#include "mining.h"
#include "blockID.h"
#include "inventory.h"
#include "worldRender.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "mobs/mobPlayer.h"
#include "deathScreen.h"
#include "daynight.h"
#include "graphics/inventoryGraphics.h"
#include "inventory.h"
#include "controls.h"
#include "titlescreen.h"

bool up;
Graphic graphics[10];
void mobHandlerUpdate(worldObject* world);

worldObject* mainGame(int mode,worldObject* CurrentWorld)
{
	consoleClear();
	clear_messages();
	if (mode != 3)// Generating world for preview doesn't require screen change
            lcdMainOnBottom();
	touchPosition touch;
	initInvGraphics();

	addInventory(PICKAXE_STONE,1);
	addInventory(AXE_STONE,1);
	addInventory(SHOVEL_STONE,1);
	addInventory(PICKAXE_DIAMOND,1);
	addInventory(AXE_DIAMOND,1);
	addInventory(SHOVEL_DIAMOND,1);

	if (mode==0) // Generate new world and reset the camera and time
	{
		iprintf("\x1b[8;1HGenerating world...\n");
		if (CurrentWorld==NULL) CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
		if (CurrentWorld==NULL)
			return NULL;
		mobsReset();
		generateWorld(CurrentWorld);
		iprintf("\x1b[8;1HA");
		CurrentWorld->CamX=0;
		CurrentWorld->CamY=0;
		CurrentWorld->timeInWorld=0;
		consoleClear();
	}
        //mode 1: Return to game
        else if (mode==2) //Load World
        {
            iprintf("\x1b[8;1HLoading world...\n");
            if (CurrentWorld==NULL) CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
            if (CurrentWorld==NULL)
		return NULL;
            mobsReset();
            loadWorld(CurrentWorld);
            iprintf("\x1b[8;1HA");
            CurrentWorld->CamX=0;
            CurrentWorld->CamY=0;
            CurrentWorld->timeInWorld=0;
            consoleClear();
                 
        }
        else if (mode == 3)//Generate world preview and return
        {
		if (CurrentWorld==NULL) CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
		if (CurrentWorld==NULL)
                    return NULL;
		mobsReset();
		generateSmallWorld(CurrentWorld);
		CurrentWorld->CamX=0;
		CurrentWorld->CamY=0;
		CurrentWorld->timeInWorld=0;
                mobHandlerUpdate(CurrentWorld);
                worldRender_Render(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY);
                return CurrentWorld;
        }
        if (isSurvival())
        {
            drawButton(21,16,9);
            drawButton(9,16,10);
            iprintf("\x1b[17;22HCrafting");
            iprintf("\x1b[17;10HSave Game");
        }
	while(1){ //Infinitely repeats until break
		scanKeys();
		touchRead(&touch);
		miningUpdate(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY,touch,keysDown());
		mobHandlerUpdate(CurrentWorld);
		updateInventory(&touch,CurrentWorld);
		update_message();
		if (keysHeld() & KEY_B && keysHeld() & KEY_DOWN)
			clear_messages();
		if (keysDown() & getKey(ACTION_MENU) || shouldQuitGame())
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
