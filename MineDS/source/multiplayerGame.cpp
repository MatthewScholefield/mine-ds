#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldgen.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "nifi.h"
#include "worldRender.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "communications.h"
#include "deathScreen.h"
void multiplayerGame(bool host)
{
	touchPosition touch;
	nifiEnable();
	iprintf("ASDF!\n");
	mobsReset();
	consoleClear();
	iprintf("\x1b[7;0H");
	worldObject* world;
	if (host)
	{	
		lcdMainOnBottom();
		iprintf("Generating World!\n");
		world = (worldObject *) calloc(1,sizeof(worldObject));
		generateWorld(world);
		Calculate_Brightness(world);
		while (!hostNifiInit()) swiWaitForVBlank();
		communicationInit(world);
	}
	else
	{
		lcdMainOnTop();
		world = (worldObject *) calloc(1,sizeof(worldObject));
		iprintf("Looking for servers\n");
		while (!clientNifiInit()) swiWaitForVBlank(); //Looks for servers, sets up Nifi, and Asks the player to join a server.
		iprintf("Joining Server!\n");
		//Next Do a HandShake and check that we are communicating with MineDS (and not another game that we might make in the future)
		if (doHandshake())
		{
			recieveWorld(world);
			iprintf("WOOT\n");
			Calculate_Brightness(world);
		}
		else return;
	}
	lcdMainOnBottom();
	world->CamX=0;
	world->CamY=0;
	iprintf("Done!\n");
	while (1)
	{
		scanKeys();
		if (keysDown() & KEY_START) break;
		touchRead(&touch);
		nifiUpdate();
		miningUpdate(world,world->CamX,world->CamY,touch,keysDown());
		if (keysDown() & KEY_START) break;
		mobHandlerUpdate(world);
		if (deathScreenUpdate()) break;
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
		worldRender_Render(world,world->CamX,world->CamY);
	}
	free(world);
	nifiDisable();
}
