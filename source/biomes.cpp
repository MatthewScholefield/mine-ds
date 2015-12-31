
#include <stdio.h>
#include <nds.h>
#include "world/World.h"
#include "blockID.h"
#include "trees.h"
#include "blocks.h"
#include "blockUpdaters/WaterUpdater.h"

void plainsBiome(World &world, int startx, int endx)
{
	int x = startx;
	int treex = startx + 4 + rand() % 5; //Trees
	int flx = startx + 4 + rand() % 5; //Flowers
	int gx = startx + 3 + rand() % 5; //Tall Grass

	while (x <= endx)
	{
		int y = findFirstBlock(world, x); // Get the first block that is not AIR...
		int endy = y + (rand() % 2) + 2;
		for (int j = y; j < endy; ++j) world.blocks[x][j] = DIRT;
		world.blocks[x][y] = GRASS;
		world.biome[x] = Biome::PLAINS;
		if (gx == x)
		{
			world.blocks[x][y - 1] = TALL_GRASS;
			gx += 1;
			if (rand() % 2 == 0) gx += rand() % 3;
		}
		if (flx == x)
		{
			if (treex != x)
				growFlower(world, x, y - 1);
			flx += 3 + rand() % 5;
		}
		if (treex == x)
		{
			growOakTree(world, x, y - 1);
			treex += 8 + rand() % 5;
		}
		++x;
	}
}

void jungleBiome(World &world, int startx, int endx)
{
	int x = startx;
	int treex = startx + 4 + rand() % 5;
	int flx = startx + 4 + rand() % 5;

	while (x <= endx)
	{
		int y = findFirstBlock(world, x); // Get the first block that is not AIR...
		int endy = y + (rand() % 2) + 2;
		for (int j = y; j < endy; ++j) world.blocks[x][j] = DIRT;
		world.blocks[x][y] = GRASS_JUNGLE;
		world.biome[x] = Biome::JUNGLE;
		if (flx == x)
		{
			world.bgblocks[x][y - 1] = LEAVES_JUNGLE;
			if (rand() % 3 == 1)
				world.bgblocks[x][y - 2] = LEAVES_JUNGLE;
			if (rand() % 8 == 1)
				flx += 1 + rand() % 3;
			else flx += 1;
		}
		if (treex == x)
		{
			growJungleTree(world, x, y - 1);
			treex += 8 + rand() % 5;
		}

		++x;
	}
}

void snowBiome(World &world, int startx, int endx)
{
	int x = startx;
	int treex = startx + 8 + rand() % 10;
	int flx = startx + 4 + rand() % 5;

	while (x <= endx)
	{
		int y = findFirstBlock(world, x); // Get the first block that is not AIR...
		int endy = y + (rand() % 2) + 2;
		for (int j = y; j < endy; ++j) world.blocks[x][j] = DIRT;
		world.blocks[x][y] = SNOW_TOP;
		world.blocks[x][y + 1] = SNOW_GRASS;
		world.biome[x] = Biome::SNOW;
		if (flx == x)
		{
			growPumpkinPatch(world, x, y - 1);
			flx += 20 + rand() % 5;
		}
		if (treex == x)
		{
			growSpruceTree(world, x, y);
			//Why no -1 on the y? Well If we did that then we would be starting on top the the snow_top block, and that looks weird!
			treex += 8 + rand() % 5;
		}
		++x;
	}
}

void desertBiome(World &world, int startx, int endx)
{
	int x = startx;
	int treex = startx + 3 + rand() % 5;
	int flx = startx + 4 + rand() % 5; //Where to Spawn Shrubs

	while (x <= endx)
	{
		int y = findFirstBlock(world, x); // Get the first block that is not AIR...
		int endy = y + (rand() % 2) + 3;
		for (int j = y; j < endy; ++j) world.blocks[x][j] = SAND;
		for (int j = endy - (1 + (rand() % 3)); j < endy; ++j) world.blocks[x][j] = SANDSTONE;
		world.biome[x] = Biome::DESERT;
		if (treex == x)
		{
			growCactus(world, x, y - 1);
			world.blocks[x][y] = SAND;
			treex += 6 + rand() % 5;
		}
		else if (flx == x)
		{
			world.blocks[x][y - 1] = SHRUB;
			world.blocks[x][y] = SAND;
			flx += 5 + rand() % 3;
		}
		if (flx == x && treex == x)
			flx += 5 + rand() % 3;

		++x;
	}
}

void mushroomBiome(World &world, int startx, int endx)
{
	int x = startx;
	int treex = startx + 8 + rand() % 10;
	int flx = startx + 1 + rand() % 3;

	while (x <= endx)
	{
		int y = findFirstBlock(world, x); // Get the first block that is not AIR...
		int endy = y + (rand() % 2) + 2;
		for (int j = y; j < endy; ++j) world.blocks[x][j] = DIRT;
		world.blocks[x][y] = MYCELIUM;
		world.biome[x] = Biome::MUSHROOM;
		if (flx == x)
		{
			world.blocks[x][y - 1] = (rand() % 2 == 1 ? MUSHROOM_BROWN : MUSHROOM_RED);
			flx += 1 + rand() % 4;
		}
		if (treex == x)
		{
			growGiantMushroom(world, x, y - 1);
			treex += 8 + rand() % 5;
		}
		++x;
	}
}
void oceanBiome(World &world, int startX, int endX)
{
	int startY = findFirstBlock(world, startX);
	int endY = findFirstBlock(world, endX);
	//Check that it is possible to properly build a water biome
	int yDiff = abs(startY - endY);
	if (yDiff > 8) return plainsBiome(world, startX, endX);
	int midX = (startX + endX) / 2 + ((startY < endY) ? -yDiff / 2 : yDiff / 2);
	int oldLowerY = startY < endY ? endY : startY;
	int lowery = oldLowerY + rand() % 4 + 5;
	for (int i = startX; i < endX; i++)
		for (int j=0; j<World::HEIGHT;j++)
			world.blocks[i][j] = AIR;
	drawLineThing(world, startX, startY, midX - 2, lowery);
	drawLineThing(world, midX - 2, lowery, midX + 2, lowery);
	drawLineThing(world, midX + 2, lowery, endX, endY);
	for (int x = startX; x <= endX; ++x)
	{
		int y = findFirstBlock(world, x); // Get the first block that is not AIR...
		if (y > (oldLowerY + 2))
		{
			int topWaterBlock = oldLowerY + 2;
			world.blocks[x][topWaterBlock] = WATER;
			setWaterLevel(world, x, topWaterBlock, (MAX_WATER_LEVEL * 5) / 6);
			for (int j = topWaterBlock + 1; world.blocks[x][j] == AIR; ++j)
			{
				world.blocks[x][j]=WATER;
				world.data[x][j]=MAX_WATER_LEVEL;
			}
		}
		int endY = y + (rand() % 2) + 2;
		for (int j = y; j < endY; ++j) world.blocks[x][j] = SAND;
	}
}
