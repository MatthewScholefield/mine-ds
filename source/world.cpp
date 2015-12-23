#include "world.h"
#include <cstdlib>
#include "worldRender.h"
#include "blockID.h"
#include "blocks.h"
#include "worldGen.h"
#include "dayNight.h"

int WorldObject::useSeed = NO_SEED;

int findFirstBlock(WorldObject &world, int x)
{
	int i;
	for (i = 0; i <= WORLD_HEIGHT; ++i)
		if (!isBlockWalkThrough(world.blocks[x][i])) return i;
	return -1;
}

void WorldObject::generateSmallWorld()//Generates one biome
{
	int y = rand() % (WORLD_HEIGHT / 4) + WORLD_HEIGHT / 4;
	int origY = y;
	int endX = rand() % 16 + 16;
	y = (rand() % 2 ? extremeMountainGen(*this, 0, y, endX) : flatGen(*this, 0, y, endX));
	generateRandomBiome(*this, 0, endX);
	calculateBrightness(*this, 8, origY);
}

void WorldObject::generate()
{
	if (gameMode == GAMEMODE_PREVIEW)
	{
		generateSmallWorld();
		return;
	}
	stopMusic();
	//Generating a world is intensive on the cpu.
	//If you don't stop the music, the whole sound engine will become stuffed up.
	int x = 0, y = rand() % (WORLD_HEIGHT / 4) + WORLD_HEIGHT / 4;
	while (x < WORLD_WIDTH)
	{
		int endX = x + rand() % 16 + 16;
		if (endX >= WORLD_WIDTH) endX = WORLD_WIDTH - 1;
		y = (rand() % 2 ? extremeMountainGen(*this, x, y, endX) : flatGen(*this, x, y, endX));
		generateRandomBiome(*this, x, endX);
		x = endX + 1;
	}
	generateBedrock(*this);
	for (x = 0; x <= WORLD_WIDTH; ++x) //Copy FG blocks to BG
		for (y = 0; y <= WORLD_HEIGHT; ++y)
			if (blocks[x][y] != AIR && !isBlockWalkThrough(blocks[x][y])) bgblocks[x][y] = blocks[x][y];
	dayNightUpdate(*this);
	calculateBrightness(*this);
}

void drawLineDown(WorldObject &world, int x, int y)
{
	int i;
	for (i = y; i < WORLD_HEIGHT; ++i)
	{
		if (i > WORLD_HEIGHT - 20 && rand() % 120 == 1) //20 blocks from bottom
			world.blocks[x][i] = DIAMOND_ORE;
		else if (i > WORLD_HEIGHT - 30 && rand() % 90 == 1) //30 blocks from bottom
			world.blocks[x][i] = GOLD_ORE;
		else if (i > WORLD_HEIGHT - 65 && rand() % 80 == 1) //65 blocks from bottom
			world.blocks[x][i] = IRON_ORE;
		else if (i > WORLD_HEIGHT - 80 && rand() % 60 == 1) //80 blocks from bottom
			world.blocks[x][i] = COAL_ORE;
		else world.blocks[x][i] = STONE;
	}
}

void drawLineThing(WorldObject &world, int x1, int y1, int x2, int y2)
{
	// if x1 == x2 or y1 == y2, then it does not matter what we set here
	int delta_x(x2 - x1);
	signed char ix((delta_x > 0) - (delta_x < 0));
	delta_x = std::abs(delta_x) << 1;

	int delta_y(y2 - y1);
	signed char iy((delta_y > 0) - (delta_y < 0));
	delta_y = std::abs(delta_y) << 1;

	drawLineDown(world, x1, y1);

	if (delta_x >= delta_y)
	{
		// error may go below zero
		int error(delta_y - (delta_x >> 1));

		while (x1 != x2)
		{
			if (error >= 0)
			{
				if (error || (ix > 0))
				{
					y1 += iy;
					error -= delta_x;
				}
				// else do nothing
			}
			// else do nothing

			x1 += ix;
			error += delta_y;

			drawLineDown(world, x1, y1);
		}
	}
	else
	{
		// error may go below zero
		int error(delta_x - (delta_y >> 1));

		while (y1 != y2)
		{
			if (error >= 0)
			{
				if (error || (iy > 0))
				{
					x1 += ix;
					error -= delta_y;
				}
				// else do nothing
			}
			// else do nothing

			y1 += iy;
			error += delta_x;

			drawLineDown(world, x1, y1);
		}
	}
}

void WorldObject::initialize()
{
	if (useSeed != NO_SEED)
	{
		seed = useSeed;
		useSeed = NO_SEED;
	}
	else
	{
		seed = time(nullptr);
		if (gameMode == GAMEMODE_PREVIEW)
			useSeed = seed;
	}
	srand(seed);
	generate();
	bool onLand = false;
	int spawnY;
	do
	{
		for (auto i = 0; i <= WORLD_HEIGHT; ++i)
			if (blocks[spawnX][i] != AIR)
			{
				onLand = (blocks[spawnX][i] != WATER);
				spawnY = i - 1;
				break;
			}
		if (!onLand)
			++spawnX;
	}
	while (!onLand);
	if (gameMode == GAMEMODE_PREVIEW)
		camCalcY = camY = spawnY * 16 - 192 / 2 - 16;
}

WorldObject::WorldObject(GameMode gameMode) : blocks { }, bgblocks{}, data{}, brightness{}

, spawnX(gameMode == GAMEMODE_PREVIEW ? 0 : (WORLD_WIDTH * 3) / 8 + rand() % (WORLD_WIDTH / 4)), camY(0), camX(gameMode == GAMEMODE_PREVIEW ? 0 : spawnX * 16 - 256 / 2)
, timeInWorld(0), worldBrightness(15), gameMode(gameMode)
, seed(NO_SEED), camCalcX(camX), camCalcY(0.0), biome { }, chestInUse{}
, furnaces{}

, reservedWater(0)
{
	initialize();
}

WorldObject::WorldObject(bool init) : blocks { }, bgblocks{}, data{}, brightness{}

, spawnX((WORLD_WIDTH * 3) / 8 + rand() % (WORLD_WIDTH / 4)), camY(0), camX(0)
, timeInWorld(0), worldBrightness(15), gameMode(GAMEMODE_PREVIEW)
, seed(NO_SEED), camCalcX(camX), camCalcY(0.0), biome { }, chestInUse{}
, furnaces{}

, reservedWater(0)
{
	if (init)
		initialize();
}
