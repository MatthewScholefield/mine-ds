/* _____________________________________________
| |
| Main.cpp |
| Part of Mine DS , by CoolAs and Ray |
| Thanks to Dirbaio! |
|_____________________________________________|
*/

#include <nds.h>
#include <stdio.h>
#include <time.h>
#include "world.h"
#include "mobs.h"
#include "particles.h"
#include "fontHandler.h"
#include "worldgen.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "worldRender.h"
#include "titlescreen.h"
#include "mainGame.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
int screen_x,screen_y;
void beginRender(int a,int b); 
void renderWorld(worldObject* world);
//Graphic graphics[9];


int main()
{
	lcdMainOnBottom();
	srand(time(NULL));
	subBgInit();
	setupFont();
	swiWaitForVBlank();
	//setSubBgTile(1,0,1);
	//iprintf("Some Random DEBUG int:%d\n",0);
	//iprintf("Some Random DEBUG bool:%s\n",isBlockWalkThrough(FLOWER_RED) ? "True" : "False");
	//iprintf("Done!\n");
	//for (i=0;i<=8;i++)
	//{
	//	loadGraphicSub(&graphics[i],true, text[i]);
	//}
	worldRender_Init();
	graphicsInit();
	while(1)
	{
	    graphicFrame();
		titlescreen();
		//mainGame(0);	
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
	return 1;
}
