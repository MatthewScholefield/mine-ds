#include "world.h"
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

static int extremeMountainGen(WorldObject &world, int startx, int starty, int endx)
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
static int flatGen(WorldObject &world, int startx, int starty, int endx)
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

static void generateBedrock(WorldObject &world)
{
	int i;
	for (i = 0; i <= WORLD_WIDTH; ++i)
	{
		world.blocks[i][WORLD_HEIGHT] = BEDROCK;
		if (!(rand() % 2)) world.blocks[i][WORLD_HEIGHT - 1] = BEDROCK;
		if (!(rand() % 4)) world.blocks[i][WORLD_HEIGHT - 2] = BEDROCK;
		if (!(rand() % 6)) world.blocks[i][WORLD_HEIGHT - 3] = BEDROCK;
		if (!(rand() % 8)) world.blocks[i][WORLD_HEIGHT - 4] = BEDROCK;
	}
}

static void generateRandomBiome(WorldObject &world, int x, int endX)
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

void generateSmallWorld(WorldObject &world)//Generates one biome
{
	int y = rand() % (WORLD_HEIGHT / 4) + WORLD_HEIGHT / 4;
	y = (rand() % 2 == 1 ? extremeMountainGen(world, 0, y, 15) : flatGen(world, 0, y, 15));
	generateRandomBiome(world, 0, 15);
	updateBrightnessAround(world, 15, y);
}

void generateWorld(WorldObject &world)
{
	if (world.gameMode == GAMEMODE_PREVIEW)
	{
		generateSmallWorld(world);
		return;
	}
	stopMusic();
	//Generating a world is intensive on the cpu.
	//If you don't stop the music, the whole sound engine will become stuffed up.
	int x = 0, y = rand() % (WORLD_HEIGHT / 4) + WORLD_HEIGHT / 4, initY = y;
	while (x < WORLD_WIDTH)
	{
		int endX = x + rand() % 16 + 16;
		if (endX >= WORLD_WIDTH) endX = WORLD_WIDTH - 1;
		y = (rand() % 2 ? extremeMountainGen(world, x, y, endX) : flatGen(world, x, y, endX));
		generateRandomBiome(world, x, endX);
		x = endX + 1;
	}
	updateBrightnessAround(world, 0, initY);
	generateBedrock(world);
	for (x = 0; x <= WORLD_WIDTH; ++x) //Copy FG blocks to BG
		for (y = 0; y <= WORLD_HEIGHT; ++y)
			if (world.blocks[x][y] != AIR && !isBlockWalkThrough(world.blocks[x][y])) world.bgblocks[x][y] = world.blocks[x][y];
}
