#include "blockupdate.h"
#include "world.h"
#include "blocks.h"
#include "blockID.h"
#include "nifi.h"
#include "communications.h"
#include "inventory.h"
#include "general.h"
//Include blockUpdaters
#include "blockupdaters/furnace.h"
#include "blockupdaters/air.h"
#include "blockupdaters/grass.h"
#include "blockupdaters/dirt.h"
#include "blockupdaters/junglegrass.h"
#include "blockupdaters/mycelium.h"
#include "blockupdaters/leaf.h"
#include "blockupdaters/redwoodLeaf.h"
#include "blockupdaters/jungleLeaf.h"

blockUpdater* blockUpdaters[10];
int numBlockUpdaters;



blockUpdater::blockUpdater()
{
}
void blockUpdater::update(worldObject* world,int x,int y,bool bg)
{
}
void blockUpdater::chanceUpdate(worldObject* world,int x,int y,bool bg)
{
  
}

void proceduralBlockUpdateInit()
{
	numBlockUpdaters = 0;
	blockUpdaters[numBlockUpdaters++] = new furnaceUpdater;
	blockUpdaters[numBlockUpdaters++] = new airUpdater;
	blockUpdaters[numBlockUpdaters++] = new grassUpdater;
	blockUpdaters[numBlockUpdaters++] = new junglegrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new myceliumUpdater;
	blockUpdaters[numBlockUpdaters++] = new dirtUpdater;
	blockUpdaters[numBlockUpdaters++] = new leafUpdater;
	blockUpdaters[numBlockUpdaters++] = new redwoodLeafUpdater;
	blockUpdaters[numBlockUpdaters++] = new jungleLeafUpdater;
}
void proceduralBlockUpdateCheck(worldObject* world, int x, int y)
{
  for (int i=0;i<numBlockUpdaters;++i)
  {
    if (world->bgblocks[x][y]==blockUpdaters[i]->blockID){
      blockUpdaters[i]->update(world,x,y,true);
      if (!(rand()%blockUpdaters[i]->chance))
      {
        blockUpdaters[i]->chanceUpdate(world,x,y,true);
      }
    }
    if (world->blocks[x][y]==blockUpdaters[i]->blockID){
      blockUpdaters[i]->update(world,x,y,false);
      if (!(rand()%blockUpdaters[i]->chance))
      {
        blockUpdaters[i]->chanceUpdate(world,x,y,false);
      }
    }
  }
}
void proceduralBlockUpdate(worldObject* world)
{
  int sx = world->CamX/16;
  int sy = world->CamY/16;
  for (int x = sx; x < (sx+16); ++x)
  {
    for (int y = sy; y < (sy+12); ++y)
    {
      proceduralBlockUpdateCheck(world,x,y);
    }
  }
}

void checkBlockPlace(int x,int y,worldObject* world,bool bg)
{
	if (!bg)
	{
		if ((world->blocks[x][y]==TORCH || world->blocks[x][y]==LADDER) && !isBlockWalkThrough(world->bgblocks[x][y]));
		else if (isBlockWalkThrough(world->blocks[x][y]) && (isBlockWalkThrough(world->blocks[x][y+1]))) // If a transparent block is on top of a transparent block, delete it
		{
			addInventory(world->blocks[x][y]);
			world->blocks[x][y] = AIR;
		}
		if (world->blocks[x][y]==CACTUS && !(world->bgblocks[x][y+1]==SAND || world->blocks[x][y+1]==CACTUS || world->blocks[x][y+1]==SAND))
		{
			addInventory(world->blocks[x][y]);
			world->blocks[x][y] = AIR;
		}
		if (world->blocks[x][y]==SNOW_TOP)
		{
			if(world->blocks[x][y+1]==GRASS)
			{
				world->blocks[x][y+1]=SNOW_GRASS;
				if (isWifi())	placeBlock(x,y+1);
			}
			if(world->bgblocks[x][y+1]==GRASS)
			{
				world->bgblocks[x][y+1]=SNOW_GRASS;
				if (isWifi())	placeBlock(x,y+1);
			}
		}

	}
	else if (bg)
	{
		if (isBlockWalkThrough(world->bgblocks[x][y]) && (isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->bgblocks[x][y+1])))
		{
			addInventory(world->bgblocks[x][y]);
			world->bgblocks[x][y] = AIR;
		}
		if (world->bgblocks[x][y]==CACTUS && !(world->bgblocks[x][y+1]==SAND || world->bgblocks[x][y+1]==CACTUS || world->blocks[x][y+1]==SAND))
		{
			addInventory(world->bgblocks[x][y]);
			world->bgblocks[x][y] = AIR;
		}
		if (world->bgblocks[x][y]==SNOW_TOP)
		{
			if(world->blocks[x][y+1]==GRASS)
			{
				world->blocks[x][y+1]=SNOW_GRASS;
				if (isWifi())	placeBlock(x,y+1);
			}
			if(world->bgblocks[x][y+1]==GRASS)
			{
				world->bgblocks[x][y+1]=SNOW_GRASS;
				if (isWifi())	placeBlock(x,y+1);
			}
		}
	}
}
void checkBlockDelete(int x,int y,worldObject* world,bool bg)
{
	int c =1;
	if (!(world->blocks[x][y]==SAND || world->bgblocks[x][y]==SAND))
	{
		while ((world->blocks[x][y-c]==CACTUS) || (world->bgblocks[x][y-c]==CACTUS))
		{
			addInventory(world->bgblocks[x][y-c]);
			addInventory(world->blocks  [x][y-c]);
			world->blocks[x][y-c]=AIR;
			world->bgblocks[x][y-c]=AIR;
			if (isWifi())	placeBlock(x,y-c);
			++c;
		}
	}
	checkBlockPlace(x,y-1,world,true);
	checkBlockPlace(x,y-1,world,false);
	if (isWifi())	placeBlock(x,y-1);
	if (world->blocks[x][y+1]==SNOW_GRASS && !(world->blocks[x][y]==SNOW_TOP || world->bgblocks[x][y]==SNOW_TOP))
	{
		world->blocks[x][y+1]=GRASS;
		if (isWifi())	placeBlock(x,y+1);
	}
	if (world->bgblocks[x][y+1]==SNOW_GRASS && !(world->blocks[x][y]==SNOW_TOP || world->bgblocks[x][y]==SNOW_TOP))
	{
		world->bgblocks[x][y+1]=GRASS;
		if (isWifi())	placeBlock(x,y+1);
	}
}

