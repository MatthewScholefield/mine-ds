/* _____________________________________________
  | 						|
  |		worldgen.cpp			|
  |     Part of Mine DS , by CoolAs and Ray	|
  |		Thanks to Dirbaio!		|
  |_____________________________________________|
*/

#include "world.h"
#include "blockID.h"
#include "blocks.h"
#include <stdio.h>
#include <cstdlib>
#include <nds.h>
#include "biomes.h"
#include "worldRender.h"
#include "files.h"
#include <string>
#include <tr1/regex>
void drawLineThing(worldObject* world,int x1,int y1,int x2,int y2);

static void zeroBlocks(worldObject* world)
{
	unsigned int i, j;

	for (i = 0; i < WORLD_WIDTH; i++)
	{
		for (j = 0; j < WORLD_HEIGHT; j++)
		{
			world->blocks[i][j] = AIR;
			world->bgblocks[i][j] = AIR;
		}
	}
}

int extremeMountainGen(worldObject* world,int startx,int starty,int endx)
{
	int y=starty;
	int x=startx;
	//We need to find a end y!
	///This can be defined!
	int endy=starty+(rand()%23-11);
	if (endy<13) endy=16;
	else if (endy>WORLD_HEIGHT/2) endy=WORLD_HEIGHT/2;
	while (x<endx-8)
	{
		if (y<8) y=8;
		int x1=x;
		x+=(rand() % 6)+4;
		int x2=x;
		int y1=y;
		if (y<=endy+12 && y>=endy-12) y+=4-(rand() % 9);
		else if (y>=endy+12) y=endy+8;
		else if (y<=endy-12) y=endy-8;
		int y2=y;
		drawLineThing(world,x1,y1,x2,y2);
	}
	drawLineThing(world,x,y,endx,endy);
	return endy;
}
int flatGen(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	int x;
	int changey=0;
	int times=rand()%3+1;
	for (x=startx;x<=endx;x++){
		if (changey==0){
			y+=rand() % 3-1;
			++times;
			if (y<5) y-=rand() %2-2;
			if (y>WORLD_HEIGHT/3) y-=rand()%2;	
			changey=rand() % 5+2;
		}
		drawLineDown(world,x,y);
		--changey;
	}
	return y;
}

	
void plotHeight(worldObject* world,int x, int y)
{
	int i;
	for (i=y;i<WORLD_HEIGHT;i++)
	{
		world->blocks[x][i]=STONE;
	}
}
void generateBedrock (worldObject* world){
	int i;
	for(i=0;i<=WORLD_WIDTH;i++)
	{
		world->blocks[i][WORLD_HEIGHT]=BEDROCK;
		if (!(rand() % 2)) world->blocks[i][WORLD_HEIGHT-1]=BEDROCK;
		if (!(rand() % 4)) world->blocks[i][WORLD_HEIGHT-2]=BEDROCK;
		if (!(rand() % 6)) world->blocks[i][WORLD_HEIGHT-3]=BEDROCK;
		if (!(rand() % 8)) world->blocks[i][WORLD_HEIGHT-4]=BEDROCK;
	}
}



void generateSmallWorld (worldObject* world)//Generates one biome
{
    int j=rand() % (WORLD_HEIGHT/4) + WORLD_HEIGHT/4;
    int oj = j;
    int i=0;
    int sizex;

    zeroBlocks(world);
    sizex=rand()%16+ 16;
    if (sizex > WORLD_WIDTH) sizex = WORLD_WIDTH;
    j = (rand() % 2==1 ? extremeMountainGen(world,i,j,i+sizex) : flatGen(world,i,j,i+sizex));
    switch (rand() % 5)
    {
	case 0:plainsBiome(world,i,i+sizex); break;
	case 1: snowBiome(world,i,i+sizex); break;
	case 2: desertBiome(world,i,i+sizex); break;
	case 3: jungleBiome(world,i,i+sizex); break;
	case 4: mushroomBiome(world,i,i+sizex); break;
    }
    
    updateBrightnessAround(world,i,j);
    updateBrightnessAround(world,0,oj);

}

void generateWorld (worldObject* world)
{
	int i,j;
	zeroBlocks(world);
	j=rand() % (WORLD_HEIGHT/4) + WORLD_HEIGHT/4;
	i=0;
	int oj=j;
	while (i<WORLD_WIDTH)
	{
		int sizex;
		sizex=rand()%16+ 16;
		if (sizex > WORLD_WIDTH) sizex = WORLD_WIDTH;
		j = (rand() % 2==1 ? extremeMountainGen(world,i,j,i+sizex) : flatGen(world,i,j,i+sizex));
		switch (rand() % 5)
		{
			case 0:plainsBiome(world,i,i+sizex); break;
			case 1: snowBiome(world,i,i+sizex); break;
			case 2: desertBiome(world,i,i+sizex); break;
			case 3: jungleBiome(world,i,i+sizex); break;
			case 4: mushroomBiome(world,i,i+sizex); break;
		}
		updateBrightnessAround(world,i,j);
		i=i+sizex+1;
	}
	updateBrightnessAround(world,0,oj);
	generateBedrock(world);
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			{
			 if (world->blocks[i][j]!=AIR && (!isBlockWalkThrough(world->blocks[i][j]) || world->blocks[i][j]==SNOW_TOP)) world->bgblocks[i][j]=world->blocks[i][j];			
			}
}
