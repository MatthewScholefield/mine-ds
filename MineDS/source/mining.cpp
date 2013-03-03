#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <nds.h>
#include <stdio.h>
#include "graphics/graphics.h"
#include "communications.h"
#include "nifi.h"
int selectedblock=0;
bool loadedgraphic=false;
Graphic topBlock;
bool incutscene = false;
bool canPlaceBlocks=true;
int framecounting=0;
int failedAttempts=0;
int last_x,last_y;
void miningSetScene(bool a)
{
	incutscene = a;
}
void calculateTopBlock()
{
	if (loadedgraphic) unloadGraphic(&topBlock);
	loadGraphicSub(&topBlock,2,selectedblock);
	loadedgraphic = true;
}
void blocksCanPlace()
{
	canPlaceBlocks=true;
	framecounting=0;
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
	else if (world->blocks[x][y]!=AIR)
	{
	 world->blocks[x][y]=AIR;
	}
	else if (world->blocks[x][y]==AIR)
	{
	 world->bgblocks[x][y]=AIR;
	}
	else return;
	//Send a WIFI Update now, if wifi is enabled!
	if (isWifi())
	{
		canPlaceBlocks=false;
		last_x=x;
		last_y=y;
		placeBlock(x,y);
	}
}
void miningUpdate(worldObject* world,int a,int b,touchPosition touch,int keys) // keys = keysDown();
{
	if (keys & KEY_TOUCH)
	{
		int x = (touch.px+a)/16;
		int y = (touch.py+b)/16;
		int a;
		if (canPlaceBlocks && !incutscene)
		{
		 setBlock(world,x,y);
		 updateBrightnessAround(world,x,y);
		}
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
	if (canPlaceBlocks==false) framecounting++;
	if (framecounting>60)
	{
		//More than a second, and no block confirm?
		placeBlock(last_x,last_y);
		framecounting=0;
		failedAttempts++;
	}
}
