#include "World.h"
#include "../blockID.h"
#include "../blocks.h"
#include <stdio.h>
#include <cstdlib>
#include <nds.h>
#include "../biomes.h"
#include "worldRender.h"
#include "../files.h"
#include "../sounds.h"
#include "../blockUpdaters/blockUpdater.h"
#include <string>

int extremeMountainGen(World &world, int startx, int starty, int endx)
{
	int y = starty;
	int x = startx;
	//We need to find a end y!
	///This can be defined!
	int endy = starty + (rand() % 23 - 11);
	if (endy < 13) endy = 16;
	else if (endy > World::HEIGHT / 2) endy = World::HEIGHT / 2;
	while (x < endx - 8)
	{
		if (y < 8) y = 8;
		int x1 = x;
		x += (rand() % 6) + 4;
		int x2 = x;
		int y1 = y;
		if (y <= endy + 12 && y >= endy - 12) y += 4 - (rand() % 9);
		else if (y >= endy + 12) y = endy + 8;
		else if (y <= endy - 12) y = endy - 8;
		int y2 = y;
		drawLineThing(world, x1, y1, x2, y2);
	}
	drawLineThing(world, x, y, endx, endy);
	return endy;
}
int flatGen(World &world, int startx, int starty, int endx)
{
	int y = starty;
	int x;
	int changey = 0;
	int times = rand() % 3 + 1;
	for (x = startx; x <= endx; ++x)
	{
		if (changey == 0)
		{
			y += rand() % 3 - 1;
			++times;
			if (y < 5) y -= rand() % 2 - 2;
			if (y > World::HEIGHT / 3) y -= rand() % 2;
			changey = rand() % 5 + 2;
		}
		drawLineDown(world, x, y);
		--changey;
	}
	return y;
}

void generateBedrock(World &world)
{
	for (int i = 0; i < World::WIDTH; ++i)
	{
		world.blocks[i][World::HEIGHT - 1] = BEDROCK;
		if (!(rand() % 2)) world.blocks[i][World::HEIGHT - 2] = BEDROCK;
		if (!(rand() % 4)) world.blocks[i][World::HEIGHT - 3] = BEDROCK;
		if (!(rand() % 6)) world.blocks[i][World::HEIGHT - 4] = BEDROCK;
		if (!(rand() % 8)) world.blocks[i][World::HEIGHT - 5] = BEDROCK;
	}
}

void generateRandomBiome(World &world, int x, int endX)
{
	switch ((Biome) (rand() % 6 + 1))
	{
	case Biome::PLAINS:
		plainsBiome(world, x, endX);
		break;
	case Biome::SNOW:
		snowBiome(world, x, endX);
		break;
	case Biome::DESERT:
		desertBiome(world, x, endX);
		break;
	case Biome::JUNGLE:
		jungleBiome(world, x, endX);
		break;
	case Biome::MUSHROOM:
		mushroomBiome(world, x, endX);
		break;
	case Biome::OCEAN:
		oceanBiome(world, x, endX);
		break;
	}
}

void generateCaves(World &world)
{
	int beginning_y = findFirstBlock(world,World::WIDTH/2);
	int y = (beginning_y + World::HEIGHT) / 2;
	int height = 1;
	
	// 0 == widening
	// 1 == sprawling
	// 2 == shortening
	int state = 0;
	
	for (int i = 0; i < World::WIDTH; ++i)
	{
		//Drill the hole
		for (int j = y - height / 2; j < y + height / 2; ++j)
		{
			
			if (world.blocks[i][j]!=BEDROCK)
				world.blocks[i][j]=AIR;
			
			
			// Generate ores in background layer
			if (rand()%4)
			{
				if ((rand()%100)==0 && world.bgblocks[i][j]==STONE)
					world.bgblocks[i][j]=DIAMOND_ORE;
				else if ((rand() % 80)==0 && world.bgblocks[i][j]==STONE)
					world.bgblocks[i][j]=GOLD_ORE;
				else if ((rand() % 40)==0 && world.bgblocks[i][j]==STONE)
					world.bgblocks[i][j]=IRON_ORE;
				else if ((rand() % 30)==0 && world.bgblocks[i][j]==STONE)
					world.bgblocks[i][j]=COAL_ORE;
			}
		}
		
		//Move along the cave, changing the height and the y position
		if (state == 0)
		{
			height += rand() % 3;
			if (height > 4 && rand() % height)
				state = 1;
		}
		else if (state == 1)
		{
			y += (rand() % 3) - 1;
			
			if ((rand() % 8)==0)
				height += (rand() % 4) - 1;
			
			if (height > 10) height = 10;
			
			if ((rand() % 30) == 0 || height < 4)
				state = 2;
		}
		else if (state == 2)
		{
			height -= rand() % 3;
			if (height < 1)
			{
				height = 1;
				y += (rand() % (World::HEIGHT / 6)) - World::HEIGHT / 12;
				i += (rand() % (World::WIDTH / 20)) + 5;
				state = 0;
			}
		}
	}
}