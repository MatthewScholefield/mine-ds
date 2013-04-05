#include "world.h"
#include "blocks.h"
#include "blockID.h"
#include "nifi.h"
#include "mining.h"
#include "communications.h"
#include "inventory.h"
bool checkBlockPlace(int x,int y,worldObject* world,bool bg)
{
	if (!bg)	
	{
		if (getSelectedblock()==SNOW_TOP)
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
		if (world->blocks[x][y+1] == GRASS || world->bgblocks[x][y+1] == SNOW_GRASS)
			world->blocks[x][y+1]=DIRT;
		if (getSelectedblock()==TORCH && !isBlockWalkThrough(world->bgblocks[x][y]));
		else if (isBlockWalkThrough(getSelectedblock()) && (isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->bgblocks[x][y+1]))) // If a transparent block is on top of a transparent block , return false
			return false;
		if (getSelectedblock()==CACTUS && !(world->bgblocks[x][y+1]==SAND || world->blocks[x][y+1]==CACTUS || world->blocks[x][y+1]==SAND))
			return false;
		return true;
	}
	else if (bg)	
	{
		if (getSelectedblock()==SNOW_TOP)
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
		if (world->bgblocks[x][y+1] == GRASS || world->blocks[x][y+1] == SNOW_GRASS)
			world->bgblocks[x][y+1]=DIRT;
		if (isBlockWalkThrough(getSelectedblock()) && (isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->bgblocks[x][y+1])))
			return false;
		if (getSelectedblock()==CACTUS && !(world->bgblocks[x][y+1]==SAND || world->bgblocks[x][y+1]==CACTUS || world->blocks[x][y+1]==SAND))
			return false;
		return true;
	}
}
void checkBlockDelete(int x,int y,worldObject* world,bool bg)
{	
	int c =1;
	if (!bg)
	{
		while (world->blocks[x][y-c]==CACTUS && world->blocks[x][y] != SAND)
		{
			world->blocks[x][y-c]=AIR;
			addInventory(CACTUS,1);
			if (isWifi())	placeBlock(x,y-c);
			c++;
		}
		if (isWifi() && checkBlockPlace(x,y-1,world,false))
			placeBlock(x,y-1);
		if (world->blocks[x][y+1]==SNOW_GRASS && !(world->blocks[x][y]==SNOW_TOP || world->bgblocks[x][y]==SNOW_TOP))
		{
			world->blocks[x][y+1]=GRASS;
			if (isWifi())	placeBlock(x,y+1);
		}
	}
	else if (bg)
	{
		if (world->bgblocks[x][y-c]==CACTUS && world->bgblocks[x][y] != SAND)
		{
			world->bgblocks[x][y-c]=AIR;
			addInventory(CACTUS,1);
			if (isWifi())	placeBlock(x,y-c);
			c++;
		}
		if (isWifi() && checkBlockPlace(x,y-1,world,true))
			placeBlock(x,y-1);
		if (world->bgblocks[x][y+1]==SNOW_GRASS && !(world->blocks[x][y]==SNOW_TOP || world->bgblocks[x][y]==SNOW_TOP))
		{
			world->bgblocks[x][y+1]=GRASS;
			if (isWifi())	placeBlock(x,y+1);
		}
	}
}
