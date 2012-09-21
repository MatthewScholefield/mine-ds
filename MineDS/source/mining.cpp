#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <nds.h>
#include <stdio.h>
#include "graphics/graphics.h"

int selectedblock=0;
bool loadedgraphic=false;
Graphic topBlock;
void calculateTopBlock()
{
	if (loadedgraphic) unloadGraphic(&topBlock);
	loadGraphicSub(&topBlock,2,selectedblock);
	loadedgraphic = true;
}
void setBlock(worldObject* world, int x,int y)
{
	bool something=true;
	if (keysHeld() & KEY_DOWN)
	{
		if (world->bgblocks[x][y]!=AIR) something=false;
	}
	if (world->blocks[x][y]==AIR && something)
	{
		if (keysHeld() & KEY_DOWN) world->bgblocks[x][y]=selectedblock;
		else world->blocks[x][y]=selectedblock;
	}
	else if (keysHeld() & KEY_DOWN)
	{
	 world->bgblocks[x][y]=AIR; 
	 world->blocks[x][y]=AIR; 
	}
	else if (world->blocks[x][y]!=AIR)
	{
	 world->blocks[x][y]=AIR;
	}
	else if (world->blocks[x][y]==AIR)
	{
	 world->bgblocks[x][y]=AIR;
	}
}
void miningUpdate(worldObject* world,int a,int b,touchPosition touch,int keys) // keys = keysDown();
{
	if (keys & KEY_TOUCH)
	{
		int x = (touch.px+a)/16;
		int y = (touch.py+b)/16;
		int a;
		setBlock(world,x,y);
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
