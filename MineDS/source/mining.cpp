#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include "mobs/hurt.h"
#include "mobs/mobHandler.h"
#include "graphics/graphics.h"
#include "communications.h"
#include "message.h"
#include "nifi.h"
#include "blockupdate.h"
#include "titlescreen.h"
#include "inventory.h"
//#include "graphics/inventoryGraphics.h"
#include "mining.h"
int selectedblock=0;
bool loadedgraphic=false;
Graphic topBlock;
extern bool LRC;
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
	int c = 1;
	if ((keysHeld() & KEY_DOWN) || selectedblock==CACTUS)
	{
		if (world->bgblocks[x][y]!=AIR) something=false;
	}
	if (world->blocks[x][y]==AIR && something)
	{
		if ((keysHeld() & KEY_DOWN))
		{
			if (subInventory(selectedblock,1))
				world->bgblocks[x][y]=selectedblock;
			checkBlockPlace(x,y,world,true);		
		}
		else 
		{
			if (subInventory(selectedblock,1))
				world->blocks[x][y]=selectedblock;
			checkBlockPlace(x,y,world,false);
		}
	}
	else if (world->blocks[x][y]!=AIR)
	{
		//if (!invFull())
		addInventory(world->blocks[x][y],1);
		world->blocks[x][y]=AIR;
		checkBlockDelete(x,y,world,false);
	}
	else if (world->blocks[x][y]==AIR)
	{
		//if (!invFull())
		addInventory(world->bgblocks[x][y],1);
		world->bgblocks[x][y]=AIR;
		checkBlockDelete(x,y,world,true);
	}
	else return;
	//Send a WIFI Update now, if wifi is enabled!
	if (isWifi())
	{
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
		char buffer[33];
		int mobNum = isMobAt(touch.px+a,touch.py+b);
		if (mobNum!=-1)
		{
			mobHandlerHurtMob(mobNum,2,PLAYER_HURT);	
		}
		else if (canPlaceBlocks && !incutscene)
		{
			setBlock(world,x,y);
			updateBrightnessAround(world,x,y);
		}
	}
	if ((keys & KEY_L && LRC==true) || (keys & KEY_X && LRC==false))
	{
		selectedblock--;
		if (selectedblock<0) selectedblock=NUM_BLOCKS;
		if (isSurvival())
		{
			while(checkInventory(selectedblock)==0)
			{
				selectedblock--;
				if (selectedblock<0) selectedblock=NUM_BLOCKS;
			}
		}
		calculateTopBlock();
	}
	else if ((keys & KEY_R && LRC==true) || (keys & KEY_B && LRC==false))
	{
		selectedblock++;
		if (selectedblock>NUM_BLOCKS) selectedblock=0;
		if (isSurvival())
		{
			while(checkInventory(selectedblock)==0)
			{
				selectedblock++;
				if (selectedblock>NUM_BLOCKS) selectedblock=0;
			}
		}
		calculateTopBlock();
	}
	if (selectedblock<0)
	{
		selectedblock=NUM_BLOCKS;
		calculateTopBlock();
	}
	else if (selectedblock>NUM_BLOCKS)
	{
		selectedblock=0;
		calculateTopBlock();
	}
	//Draw the selected block
	if (selectedblock!=0)
	{
		showGraphic(&topBlock,0,48);
	}	
	if (canPlaceBlocks==false) framecounting++;
	if (framecounting>60)
	{
		//More than a second, and no block confirm?
		placeBlock(last_x,last_y);
		framecounting=0;
		failedAttempts++;
	}
}

