
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
		world.biome[x] = BIOME_PLAINS;
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
		world.biome[x] = BIOME_JUNGLE;
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
		world.biome[x] = BIOME_SNOW;
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
		world.biome[x] = BIOME_DESERT;
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
		world.biome[x] = BIOME_MUSHROOM;
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
void oceanBiome(World &world, int startx, int endx)
{
	int x = startx;
	int starty = findFirstBlock(world, startx);
	int endy = findFirstBlock(world, endx);
	//Check that it is possible to properly build a water biome
	int ydiff = abs(starty - endy);
	if (ydiff > 8 ) return plainsBiome(world,startx,endx);
	int midx = (startx + endx) / 2 + ((starty < endy) ? -ydiff/2 : ydiff/2);
	int olowery = starty < endy ? endy : starty;
	int lowery = olowery + rand() % 4 + 5;
	for (int i=startx; i < endx; i++)
		for (int j=0; j<World::HEIGHT;j++)
			world.blocks[i][j] = AIR;
	drawLineThing(world,startx, starty,midx-2,lowery);
	drawLineThing(world,midx-2,lowery,midx+2,lowery);
	drawLineThing(world,midx+2,lowery,endx,endy);
	while (x <= endx)
	{
		int y = findFirstBlock(world, x); // Get the first block that is not AIR...
		if ( y > (olowery + 2) )
		{
			int j=olowery + 2;
			while (world.blocks[x][j]==AIR)
			{
				world.blocks[x][j]=WATER;
				world.data[x][j]=MAX_WATER_LEVEL;
				++j;
			}
		}
		int endy = y + (rand() % 2) + 2;
		for (int j = y; j < endy; ++j) world.blocks[x][j] = SAND;
		++x;
	}
}
