#pragma once
#include <time.h>
#include <algorithm>
#include "../Furnace.h"
#include "../inventory.h"

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
/*
	static const int HEIGHTPX = HEIGHT * 16;
	static const int WIDTHPX = HEIGHT * 16;
 */
class World
{
	static const int NO_SEED = 0;
	static int useSeed;

	void generateSmallWorld();
	void generate();
	void initialize();
public:
	static const int HEIGHT = 128, WIDTH = 1024, BLOCK_PX = 16;
	static const int CHEST_SLOTS = 30, MAX_CHESTS = 30, MAX_FURNACES = 30;
	short blocks[WIDTH][HEIGHT];
	short bgblocks[WIDTH][HEIGHT];
	int data[WIDTH][HEIGHT];
	short brightness[WIDTH][HEIGHT];

	int spawnX;
	int camY;
	int camX;
	int timeInWorld;
	int sunBrightness;
	GameMode gameMode;
	int seed; //The random number seed used to generate the world
	double camCalcX;
	double camCalcY;
	Biome biome[WIDTH];
	bool chestInUse[MAX_CHESTS];
	Inventory chests[MAX_CHESTS];
	Furnace furnaces[MAX_FURNACES];
	int reservedWater;

	World(GameMode gameMode = GAMEMODE_PREVIEW);
	World(bool init);

	~World()
	{
	}
};

int findFirstBlock(World &world, int x);
int findFirstBiomeBlock(World &world, int x);
void drawLineDown(World &world, int x, int y);
void drawLineThing(World &world, int x1, int y1, int x2, int y2);
