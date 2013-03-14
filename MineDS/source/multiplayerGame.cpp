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
#include "message.h"
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
		consoleClear();
		unsigned short buffer[100];
		int server_id = getServerID();	
		sprintf((char *)buffer,"Server ID: %d\n", server_id);			
		print_message((char *)buffer);	
	}
	else
	{
		lcdMainOnTop();
		world = (worldObject *) calloc(1,sizeof(worldObject));
		int i,j;
		for (i=0;i<=WORLD_WIDTH;i++)
			for(j=0;j<=WORLD_HEIGHT;j++)
			{
				world->blocks[i][j]=BEDROCK;
				world->bgblocks[i][j]=BEDROCK;
			}		
		iprintf("Looking for servers\n");
		while (!clientNifiInit()) swiWaitForVBlank(); //Looks for servers, sets up Nifi, and Asks the player to join a server.
		iprintf("Joining Server!\n");
		//Next Do a HandShake and check that we are communicating with MineDS (and not another game that we might make in the future)
		if (doHandshake())
		{
			recieveWorld(world);
			Calculate_Brightness(world);
		}
		else return;
		consoleClear();
		unsigned short buffer[100];
		int client_id = getClientID();	
		sprintf((char *)buffer,"%d joined the game.\n", client_id);			
		print_message((char *)buffer);	
	}
	lcdMainOnBottom();
	world->CamX=0;
	world->CamY=0;
	while (1)
	{
		scanKeys();
		if (keysDown() & KEY_START) break;
		recieveWorldUpdate();
		touchRead(&touch);
		nifiUpdate();
		miningUpdate(world,world->CamX,world->CamY,touch,keysDown());	
		update_message();
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
