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
	mobsReset();
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
		if (keysDown() & KEY_A) Calculate_Brightness(CurrentWorld);
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
