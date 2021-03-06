#pragma once
#include "../world/World.h"
#define NUM_UPDATERS 29
#define NO_CHANCE 99999
#define SOIL_CHANCE_UPDATE 1000
#define SAPLING_CHANCE_UPDATE 2000
#define LEAF_CHANCE_UPDATE 800
#define LEAF_SQUARE_RADIUS 2
#define CACTUS_CHANCE_UPDATE 1000

class BlockUpdater
{
public:

	BlockUpdater() { }
	int chance = NO_CHANCE;

	virtual void chanceUpdate(World &world, int x, int y, bool bg) { }
	virtual bool update(World &world, int x, int y, bool bg)
	{
		return false;
	}
	int timer = 0;
	virtual ~BlockUpdater() { }
};

typedef struct
{
	int x;
	int y;
	bool bg;
	int TimeToLive;
	bool chance;
}BlockUpdateInfo;



void proceduralBlockUpdateInit();
void proceduralBlockUpdate(World &world);
void updateAround(World &world, int x, int y);
void updateSingleBlock(World &world, int x, int y, bool bg = false, int timeToUpdate = 1);
