
#include <stdio.h>
#include <nds.h>
#include "world.h"
#include "blockID.h"
#include "trees.h"
#include "blocks.h"
int findFirstBlock(worldObject* world,int x)
{
	int i;
	for (i=0;i<=WORLD_HEIGHT;i++)
		if (!isBlockWalkThrough(world->blocks[x][i])) return i;
	return -1;
}
void plainsBiome(worldObject* world, int startx,int endx)
{
	int x=startx;
	int treex=startx + 4 + rand()%5;
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
		x++;
	}
}
void snowBiome(worldObject* world,int startx,int endx)
{
	int x=startx;
	int treex=startx + 4 + rand()%5;
	int y;
	
	while (x<=endx)
	{
		y=findFirstBlock(world,x); // Get the first block that is not AIR...
		int endy=y+(rand() % 2) + 2;
		for (int j=y; j<endy;j++) world->blocks[x][j]=DIRT;
		world->blocks[x][y]=SNOW_TOP;
		world->blocks[x][y+1]=SNOW_GRASS;
		if (treex==x)
		{
			growOakTree(world,x,y); 
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
		x++;
	}
}
