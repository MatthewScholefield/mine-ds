#include "world.h"
#include "blocks.h"
#include "blockID.h"
#include "nifi.h"
#include "communications.h"
#include "mining.h" //For variable BL
extern int BL;
void checkBlockPlace(int x,int y,worldObject* world,bool bg)
{
	if (!bg)	
	{
		if (world->blocks[x][y]==TORCH && !isBlockWalkThrough(world->bgblocks[x][y]));
		else if (isBlockWalkThrough(world->blocks[x][y]) && (isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->bgblocks[x][y+1]))) // If a transparent block is on top of a transparent block, delete it
		{
			world->blocks[x][y] = AIR;
		}
		if (world->blocks[x][y]==CACTUS && !(world->bgblocks[x][y+1]==SAND || world->blocks[x][y+1]==CACTUS || world->blocks[x][y+1]==SAND))
		{
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
			world->bgblocks[x][y] = AIR;
		}
		if (world->bgblocks[x][y]==CACTUS && !(world->bgblocks[x][y+1]==SAND || world->bgblocks[x][y+1]==CACTUS || world->blocks[x][y+1]==SAND))
		{
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
		while ((world->blocks[x][y-c]==CACTUS) || (world->bgblocks[x][y-c]==CACTUS))
                	{
                	        world->blocks[x][y-c]=AIR;
                	        world->bgblocks[x][y-c]=AIR;
				if (isWifi())	placeBlock(x,y-c);
                                BL++;//Adds one to Block Limit
               	        	c++;
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
