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
#include "titlescreen.h"
#define NUM_BLOCKS 47
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
		if ((keysHeld() & KEY_DOWN) || (selectedblock==CACTUS && (world->bgblocks[x][y+1]==SAND || world->bgblocks[x][y+1]==CACTUS)))
                {
                    world->bgblocks[x][y]=selectedblock;
                    if (selectedblock==SNOW_TOP && (world->bgblocks[x][y+1]==GRASS || world->bgblocks[x][y+1]==DIRT))
                {
                        world->bgblocks[x][y+1]=SNOW_GRASS;
                }
                }
                else if ((selectedblock==RED_FLOWER || selectedblock==YELLOW_FLOWER || selectedblock==TORCH) && world->bgblocks[x][y+1]!=AIR && world->bgblocks[x][y+1]!=RED_FLOWER && world->bgblocks[x][y+1]!=YELLOW_FLOWER && world->bgblocks[x][y+1]!=TORCH && world->bgblocks[x][y+1]!=SNOW_TOP)
                    world->blocks[x][y]=selectedblock;
                else if (selectedblock==SNOW_TOP && (world->blocks[x][y+1]==GRASS || world->blocks[x][y+1]==DIRT))
                {
                        world->blocks[x][y]=selectedblock;
                        world->blocks[x][y+1]=SNOW_GRASS;
                }
		else if (selectedblock!=CACTUS && selectedblock!=SNOW_TOP && selectedblock!=RED_FLOWER && selectedblock!=YELLOW_FLOWER && selectedblock!=TORCH)
                {
                    world->blocks[x][y]=selectedblock;
                }
	}
	else if (world->blocks[x][y]!=AIR)
	{
                if (world->blocks[x][y]==SNOW_TOP)
                {
                        world->blocks[x][y+1]=GRASS;
                        if (world->bgblocks[x][y]==SNOW_TOP)
                            world->bgblocks[x][y]=AIR;
                }
                world->blocks[x][y]=AIR;
                if ((world->blocks[x][y-1]==SNOW_TOP) || (world->bgblocks[x][y-1]==SNOW_TOP))
                {
                        world->blocks[x][y-1]=AIR;
                        world->bgblocks[x][y-1]=AIR;
                }
	}
	else if (world->blocks[x][y]==AIR)
	{
                if (world->bgblocks[x][y]==SNOW_TOP)
                    world->bgblocks[x][y+1]=GRASS;
                world->bgblocks[x][y]=AIR;
                while ((world->blocks[x][y-c]==CACTUS) || (world->bgblocks[x][y-c]==CACTUS))
                {
                        world->blocks[x][y-c]=AIR;
                        world->bgblocks[x][y-c]=AIR;
                        c++;
                }
                if ((world->blocks[x][y-1]==SNOW_TOP) || (world->bgblocks[x][y-1]==SNOW_TOP))
                {
                        world->blocks[x][y-1]=AIR;
                        world->bgblocks[x][y-1]=AIR;
                }
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
		char buffer[33];
		int mobNum = isMobAt(touch.px+a,touch.py+b);
		if (mobNum!=-1)
		{
			mobHandlerHurtMob(mobNum,1,PLAYER_HURT);	
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
		calculateTopBlock();
	}
	else if ((keys & KEY_R && LRC==true) || (keys & KEY_B && LRC==false))
	{
		selectedblock++;
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
