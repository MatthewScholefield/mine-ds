#include <nds.h>
#include "world.h"
#include "worldgen.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "worldRender.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
void mainGame(int Mode)
{
	lcdMainOnBottom();
	touchPosition touch;
	worldObject* CurrentWorld;
	int i,j;
	if (Mode==0) // Generate new world!
	{
		CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
		for (i=0;i<=WORLD_WIDTH;i++)
			for(j=0;j<=WORLD_HEIGHT;j++)
				CurrentWorld->blocks[i][j]=AIR;
		generateWorld(CurrentWorld);
		Calculate_Brightness(CurrentWorld);
	}	
	CurrentWorld->CamX=0;
	CurrentWorld->CamY=0;
	while(1){
		scanKeys();
		touchRead(&touch);
		miningUpdate(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY,touch,keysDown());
		if (keysDown() & KEY_START) break;
		if(keysHeld() & KEY_DOWN) CurrentWorld->CamY++;
		else if (keysHeld() & KEY_UP) CurrentWorld->CamY--;
		if (keysHeld() & KEY_LEFT) CurrentWorld->CamX--;
		else if (keysHeld() & KEY_RIGHT) CurrentWorld->CamX++;
		if (keysDown() & KEY_A) Calculate_Brightness(CurrentWorld);
		if( CurrentWorld->CamX <0) CurrentWorld->CamX = 0;
		if (CurrentWorld->CamY<0) CurrentWorld->CamY = 0;
		if( CurrentWorld->CamX>WORLD_WIDTH*16-256) CurrentWorld->CamX = WORLD_WIDTH*16-256;
		if (CurrentWorld->CamY>(WORLD_HEIGHT+1)*16-192) CurrentWorld->CamY = (WORLD_HEIGHT+1)*16-192;
		//if (up) texty--;
		//else texty++;
		//if (texty>96) up=true;
		//else if (texty<64) up=false;
		worldRender_Render(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
	}
	free(CurrentWorld);
}
