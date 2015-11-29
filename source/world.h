#pragma once
#include <time.h>
#include <algorithm>
#include "Furnace.h"
#include "inventory.h"

//Do not change WORLD_HEIGHT or WORLD_WIDTH unless you are prepared to update assembly code
//Assembly functions in worldfunctions.s depend on these values being as they are
//Also do not change order of arrays in worldObject struct, just append things to the bottom
#define WORLD_HEIGHT	(128)
#define WORLD_WIDTH	(512)
#define WORLD_HEIGHTPX	(WORLD_HEIGHT * 16)
#define WORLD_WIDTHPX	(WORLD_WIDTH * 16)

#define CHEST_SLOTS 30
#define MAX_CHESTS 30
#define MAX_FURNACES 30
#define INDEX_BLOCK_ID 0
#define INDEX_AMOUNT 1

enum GameMode
{
	GAMEMODE_CREATIVE = 1,
	GAMEMODE_SURVIVAL = 2,
	GAMEMODE_PREVIEW = 3
};

enum Biome
{
	BIOME_PLAINS = 1,
	BIOME_SNOW = 2,
	BIOME_DESERT = 3,
	BIOME_JUNGLE = 4,
	BIOME_MUSHROOM = 5,
	BIOME_OCEAN = 6
};

class WorldObject
{
public:
	int blocks[WORLD_WIDTH + 1][WORLD_HEIGHT + 1];
	int data[WORLD_WIDTH + 1][WORLD_HEIGHT + 1];
	int brightness[WORLD_WIDTH + 1][WORLD_HEIGHT + 1];
	int lightemit[WORLD_WIDTH + 1][WORLD_HEIGHT + 1];
	int sun[WORLD_WIDTH + 1][WORLD_HEIGHT + 1];
	int bgblocks[WORLD_WIDTH + 1][WORLD_HEIGHT + 1];
	int camY;
	int camX;
	int timeInWorld;
	int worldBrightness;
	GameMode gameMode;
	int seed; //The random number seed used to generate the world
	double camCalcX;
	double camCalcY;
	Biome biome[WORLD_WIDTH + 1];
	bool chestInUse[MAX_CHESTS];
	Inventory chests[MAX_CHESTS];
	Furnace furnaces[MAX_FURNACES];
	int reservedWater;
	int spawnX;

	WorldObject(GameMode gameMode = GAMEMODE_PREVIEW) : blocks { }, data{}, brightness{}, lightemit{}, sun{}, bgblocks{}

	, camY(0), camX(0), timeInWorld(0), worldBrightness(0), gameMode(gameMode)
	, seed(1), camCalcX(0.0), camCalcY(0.0), biome { }, chestInUse{}

	, furnaces{}

	, reservedWater(0), spawnX(0) { }

	~WorldObject()
	{
	}
};

int findFirstBlock(WorldObject &world, int x);
int findFirstBiomeBlock(WorldObject &world, int x);
void drawLineDown(WorldObject &world, int x, int y);
void drawLineThing(WorldObject &world, int x1, int y1, int x2, int y2);
