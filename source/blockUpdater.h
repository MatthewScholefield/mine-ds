#pragma once
#include "world.h"
#define NUM_UPDATERS 28
#define NO_CHANCE 99999
#define SOIL_CHANCE_UPDATE 1000
#define SAPLING_CHANCE_UPDATE 2000
#define LEAF_CHANCE_UPDATE 800
#define LEAF_SQUARE_RADIUS 4
#define CACTUS_CHANCE_UPDATE 1000

class BlockUpdater
{
public:
	BlockUpdater();
	int chance = NO_CHANCE;
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
	virtual bool update(WorldObject* world, int x, int y, bool bg);
	int timer = 0;
};

typedef struct
{
	int x;
	int y;
	bool bg;
	int TimeToLive;
}BlockUpdateInfo;



void proceduralBlockUpdateInit();
void proceduralBlockUpdate(WorldObject* world);
void updateAround(WorldObject *world, int x, int y);
