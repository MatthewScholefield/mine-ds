#pragma once
#include "world.h"
#define NO_CHANCE 99999
#define SOIL_CHANCE_UPDATE 1000
#define SAPLING_CHANCE_UPDATE 2000
#define LEAF_CHANCE_UPDATE 800
#define LEAF_SQUARE_RADIUS 4

class blockUpdater {
public:
	int blockID = 15;
	blockUpdater();
	int chance = NO_CHANCE;
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
	virtual void update(WorldObject* world, int x, int y, bool bg);
};
void proceduralBlockUpdateInit();
void proceduralBlockUpdate(WorldObject* world);
