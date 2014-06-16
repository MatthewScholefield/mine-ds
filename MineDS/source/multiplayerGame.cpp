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
#include "mobs/mobPlayer.h"
#include "communications.h"
#include "deathScreen.h"
#include "general.h"
#include "daynight.h"
#include "controls.h"
worldObject* multiplayerGame(bool host,worldObject* world4)
{
	touchPosition touch;
	nifiEnable();
	iprintf("ASDF!\n");
	mobsReset();
	consoleClear();
	clear_messages();
	printXY(0, 7, "");
	if (host)
	{	
		lcdMainOnBottom();
		iprintf("Generating World!\n");
		if (world4==NULL) world4 = (worldObject *) calloc(1,sizeof(worldObject));
		if (world4==NULL) return NULL;
		generateWorld(world4);
		while (!hostNifiInit()) swiWaitForVBlank();
		communicationInit(world4);
		consoleClear();
		unsigned short buffer[100];
		int server_id = getServerID();	
		sprintf((char *)buffer,"Server ID: %d\n", server_id);			
		print_message((char *)buffer);	
		world4->timeInWorld=0;
	}
	else
	{
		lcdMainOnTop();
		if (world4==NULL)
			world4 = (worldObject *) calloc(1,sizeof(worldObject));
		if (world4==NULL)
			return NULL;
		int i,j;
		for (i=0;i<=WORLD_WIDTH;i++)
			for(j=0;j<=WORLD_HEIGHT;j++)
			{
				world4->blocks[i][j]=BEDROCK;
				world4->bgblocks[i][j]=BEDROCK;
			}		
		iprintf("Looking for servers\n");
		while (!clientNifiInit()) swiWaitForVBlank(); //Looks for servers, sets up Nifi, and Asks the player to join a server.
		iprintf("Joining Server!\n");
		//Next Do a HandShake and check that we are communicating with MineDS (and not another game that we might make in the future)
		if (doHandshake())
		{
			recieveWorld(world4);
		}
		else return NULL;
		consoleClear();
		unsigned short buffer[100];
		int client_id = getClientID();	
		sprintf((char *)buffer,"%d joined the game.\n", client_id);			
		print_message((char *)buffer);	
	}
	lcdMainOnBottom();
	world4->CamX=0;
	world4->CamY=0;
	while (1)
	{
		scanKeys();
		if (keysDown() & getKey(ACTION_MENU)) break;
		recieveWorldUpdate();
		touchRead(&touch);
		nifiUpdate();
		miningUpdate(world4,world4->CamX,world4->CamY,touch,keysDown());	
		update_message();
		mobHandlerUpdate(world4);
		if (keysDown() & getKey(ACTION_MENU) || shouldQuitGame()) break;
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
		if (host) timeUpdate(world4);
		worldRender_Render(world4,world4->CamX,world4->CamY);
	}
	nifiDisable();
	return world4;
}
