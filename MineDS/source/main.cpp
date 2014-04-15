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
#include "blocks.h"
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
#include "mobs/mobHandler.h"
#include "crafting.h"
#include "nifi.h"
#include "sounds.h"
#include "files.h"
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include <string>
int screen_x,screen_y;
void beginRender(int a,int b); 
void renderWorld(worldObject* world);
//Graphic graphics[9];


int main()
{
	defaultExceptionHandler();
	//lcdMainOnBottom();
	srand(time(NULL));
	subBgInit();
        drawBackground();
	setupFont();
	nifiInit();
	mmStop();
	swiWaitForVBlank();
	initBlockProperties();
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
	mobHandlerInit();
	worldRender_LoadSprites();
	craftingInit();
        //openFiles();
        /*std::string in = getLine();
        std::string test ("hello yes"); //name.charat(index)
        bool diff = in.compare(test)==0;
        iprintf(diff?"NO":"YES");*/
	while(1)
	{
	    	graphicFrame();
		oamClear(&oamSub,0,127);
		titlescreen();
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
	return 1;
}
