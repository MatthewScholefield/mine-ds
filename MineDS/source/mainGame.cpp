#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldgen.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "worldRender.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
int texty;
bool up;
char text[]={'M','i','n','e','c','r','a','f','t'};
Graphic graphics[9];
void mobHandlerUpdate(worldObject* world);
void mainGame(int Mode)
{
	consoleClear();
	texty=64;
	lcdMainOnBottom();
	iprintf("\x1b[7;0HGenerating world...\n");
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
	for (i=0;i<=8;i++)
	{
		loadGraphicSub(&graphics[i],true, text[i]);
	}
	iprintf("Done!\n");
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
		if (up) texty--;
		else texty++;
		if (texty>96) up=true;
		else if (texty<64) up=false;
		for (i=0;i<=8;i++)
		{
			showGraphic(&graphics[i],i*8+128,texty);
		}
		worldRender_Render(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY);
		mobHandlerUpdate(CurrentWorld);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
	}
	free(CurrentWorld);
}
