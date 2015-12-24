#include "WorldObject.h"
#include "blockID.h"
#include "blocks.h"
#include <stdio.h>
#include <cstdlib>
#include <nds.h>
#include "biomes.h"
#include "worldRender.h"
#include "files.h"
#include "sounds.h"
#include "blockUpdaters/blockUpdater.h"
#include <string>

int extremeMountainGen(WorldObject &world, int startx, int starty, int endx)
{
	int y = starty;
	int x = startx;
	//We need to find a end y!
	///This can be defined!
	int endy = starty + (rand() % 23 - 11);
	if (endy < 13) endy = 16;
	else if (endy > WORLD_HEIGHT / 2) endy = WORLD_HEIGHT / 2;
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
int flatGen(WorldObject &world, int startx, int starty, int endx)
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
			if (y > WORLD_HEIGHT / 3) y -= rand() % 2;
			changey = rand() % 5 + 2;
		}
		drawLineDown(world, x, y);
		--changey;
	}
	return y;
}

void generateBedrock(WorldObject &world)
{
	for (int i = 0; i < WORLD_WIDTH; ++i)
	{
		world.blocks[i][WORLD_HEIGHT - 1] = BEDROCK;
		if (!(rand() % 2)) world.blocks[i][WORLD_HEIGHT - 2] = BEDROCK;
		if (!(rand() % 4)) world.blocks[i][WORLD_HEIGHT - 3] = BEDROCK;
		if (!(rand() % 6)) world.blocks[i][WORLD_HEIGHT - 4] = BEDROCK;
		if (!(rand() % 8)) world.blocks[i][WORLD_HEIGHT - 5] = BEDROCK;
	}
}

void generateRandomBiome(WorldObject &world, int x, int endX)
{
	switch (rand() % 6 + 1)
	{
	case BIOME_PLAINS:
		plainsBiome(world, x, endX);
		break;
	case BIOME_SNOW:
		snowBiome(world, x, endX);
		break;
	case BIOME_DESERT:
		desertBiome(world, x, endX);
		break;
	case BIOME_JUNGLE:
		jungleBiome(world, x, endX);
		break;
	case BIOME_MUSHROOM:
		mushroomBiome(world, x, endX);
		break;
	case BIOME_OCEAN:
		oceanBiome(world, x, endX);
		break;
	}
}
