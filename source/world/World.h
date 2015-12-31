#pragma once
#include <time.h>
#include <algorithm>
#include "../Furnace.h"
#include "../inventory.h"

enum class Biome
{
	PLAINS = 1,
	SNOW = 2,
	DESERT = 3,
	JUNGLE = 4,
	MUSHROOM = 5,
	OCEAN = 6
};

class World
{
	static const int NO_SEED = 0;
	static int useSeed;

	void generateSmallWorld();
	void generate();
	void initialize();
public:

	enum class Mode
	{
		CREATIVE = 1,
		SURVIVAL = 2,
		PREVIEW = 3
	};

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
	Mode gameMode;
	int seed; //The random number seed used to generate the world
	double camCalcX;
	double camCalcY;
	Biome biome[WIDTH];
	bool chestInUse[MAX_CHESTS];
	Inventory chests[MAX_CHESTS];
	Furnace furnaces[MAX_FURNACES];
	int reservedWater;

	World(Mode gameMode = Mode::PREVIEW);
	World(bool init);

	~World() { }
};

int findFirstBlock(World &world, int x);
int findFirstBiomeBlock(World &world, int x);
void drawLineDown(World &world, int x, int y);
void drawLineThing(World &world, int x1, int y1, int x2, int y2);
