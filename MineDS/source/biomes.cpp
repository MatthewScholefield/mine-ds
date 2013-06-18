
#include <stdio.h>
#include <nds.h>
#include "world.h"
#include "blockID.h"
#include "trees.h"
#include "blocks.h"
#include "world.h"
void plainsBiome(worldObject* world, int startx,int endx)
{
	int x=startx;
	int treex=startx + 4 + rand()%5; //Trees
	int flx=startx + 4 + rand()%5; //Flowers
	int gx=startx + 3 + rand()%5; //Tall Grass
	int y;
	
	while (x<=endx)
	{
		y=findFirstBlock(world,x); // Get the first block that is not AIR...
		int endy=y+(rand() % 2) + 2;
		for (int j=y; j<endy;j++) world->blocks[x][j]=DIRT;
		world->blocks[x][y]=GRASS;
		if (treex==x)
		{
			growNormalTree(world,x,y-1,false);
			treex+= 8 + rand()%5;
		}
		else if (flx==x)
		{
			growFlower(world,x,y-1);
			flx+= 3 + rand()%5;
		}
		else if (gx==x)
		{
			world->blocks[x][y-1]=TALL_GRASS;
			gx+= 1 + rand()%7;
		}
		if (flx==x && treex==x)
			flx+= 3 + rand()%5;
		if (gx==x && treex==x)
			gx+= 1 + rand()%7;
		if (gx==x && flx==x)
			gx+= 1 + rand()%7;
		x++;
	}
}
void jungleBiome(worldObject* world, int startx,int endx)
{
	int x=startx;
	int treex=startx + 4 + rand()%5;
	int flx=startx + 4 + rand()%5;
	int y;
	bool previous;
	bool previous2;
	
	while (x<=endx)
	{
		y=findFirstBlock(world,x); // Get the first block that is not AIR...
		int endy=y+(rand() % 2) + 2;
		for (int j=y; j<endy;j++) world->blocks[x][j]=DIRT;
		if (flx==x)
		{
			world->bgblocks[x][y-1]=JUNGLE_LEAF;
			if (rand()%3 == 1)
				world->bgblocks[x][y-2] = JUNGLE_LEAF;
			if (rand()%8 == 1)
				flx += 1+rand()%3;
			else flx += 1;
		}
		world->blocks[x][y]=JUNGLE_GRASS;
		if (treex==x)
		{
			growJungleTree(world,x,y-1);
			treex+= 8 + rand()%5;
		}
		
		x++;
	}
}
void snowBiome(worldObject* world,int startx,int endx)
{
	int x=startx;
	int treex=startx + 8 + rand()%10;
	int flx=startx + 4 + rand()%5;
	int y;
	
	while (x<=endx)
	{
		y=findFirstBlock(world,x); // Get the first block that is not AIR...
		int endy=y+(rand() % 2) + 2;
		for (int j=y; j<endy;j++) world->blocks[x][j]=DIRT;
		world->blocks[x][y]=SNOW_TOP;
		world->blocks[x][y+1]=SNOW_GRASS;
		if (flx==x)
		{
			growPumpkinPatch(world,x,y-1);
			flx+= 10 + rand()%5;
		}
		if (treex==x)
		{
			growOakTree(world,x,y); 
			world->blocks[x][y]=AIR;
			//Why no -1 on the y? Well If we did that then we would be starting on top the the snow_top block, and that looks weird!
			treex+= 8 + rand()%5;
		}
		x++;
	}
}
void desertBiome(worldObject* world,int startx,int endx)
{
	int x=startx;
	int treex=startx + 3 + rand()%5;
	int y;
	int flx=startx + 4 + rand()%5; //Where to Spawn Shrubs
	
	while (x<=endx)
	{
		y=findFirstBlock(world,x); // Get the first block that is not AIR...
		int endy=y+(rand() % 2) + 3;
		for (int j=y; j<endy;j++) world->blocks[x][j]=SAND;
		for (int j=endy-(1+(rand()%3));j<endy;j++) world->blocks[x][j]=SANDSTONE;
		if (treex==x)
		{
			growCactus(world,x,y-1);
			treex+= 6 + rand()%5;
		}
		else if (flx==x)
		{
			world->blocks[x][y-1]=SHRUB;
			flx+= 5 + rand()%3;
		}
		if (flx==x && treex==x)
			flx+= 5 + rand()%3;

		x++;
	}
}
