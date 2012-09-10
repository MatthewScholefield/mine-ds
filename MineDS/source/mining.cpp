#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <nds.h>
#include <stdio.h>
#include "graphics/graphics.h"

int selectedblock=0;
Graphic topBlock;
void calculateTopBlock()
{
	unloadGraphic(&topBlock);
	loadGraphicSub(&topBlock,2,selectedblock);
}
void miningUpdate(worldObject* world,int a,int b,touchPosition touch,int keys)
{
	if (keys & KEY_TOUCH)
	{
		int x = (touch.px+a)/16;
		int y = (touch.py+b)/16;
		int a;
		world->blocks[x][y]=selectedblock;
		updateBrightnessAround(world,x,y);
	}
	if (keys & KEY_L)
	{
		selectedblock--;
		calculateTopBlock();
	}
	else if (keys & KEY_R)
	{
		selectedblock++;
		calculateTopBlock();
	}
	if (selectedblock<=0) selectedblock=0;
	if (selectedblock!=0) showGraphic(&topBlock,0,0);
}
