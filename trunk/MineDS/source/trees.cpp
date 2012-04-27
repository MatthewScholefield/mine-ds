#include <stdio.h>
#include <nds.h>
#include "world.h"
#include "blocks.h"
#include "blockID.h"


void leafVirus(worldObject* world,int x, int y,int amount,bool down)
{
	if (amount<=0) return;
	world->blocks[x][y]=LEAF;
	leafVirus(world,x+1,y,amount-1,down);
	leafVirus(world,x-1,y,amount-1,down);
	leafVirus(world,x,y-1,amount-1,down);
	if (down) leafVirus(world,x,y+1,amount-1,down);
}
void growNormalTree(worldObject* world,int x,int y,bool birch)
{
	int blockID=birch==true ? BIRCH_WOOD:LOG;
	int height = 2 + rand() % 2;
	int i;
	for (i=y;i>=y-height;i--)
	{
		world->blocks[x][i]=blockID; // Draw the trunk.
	}
	//Now for the leaves...
	leafVirus(world,x,i,2+(rand()%2),false);
}
void growOakTree(worldObject* world,int x,int y)
{
	int height = 2 + rand() % 2;
	int blockID=OAK_WOOD;
	int i;
	for (i=y;i>=y-height;i--)
	{
		world->blocks[x][i]=blockID; // Draw the trunk.
	}
	leafVirus(world,x,y-height,2,false);
	y=i;
	for (i=y;i>=y-height;i--)
	{
		world->blocks[x][i]=blockID; // Draw the trunk.
	}
	leafVirus(world,x,i,2+rand()%2,false);
}
void growCactus(worldObject* world,int x,int y)
{
	int height = rand()%3;
	int j;
	for (j=y-height;j<=y;j++) world->blocks[x][j]=CACTUS;
}
