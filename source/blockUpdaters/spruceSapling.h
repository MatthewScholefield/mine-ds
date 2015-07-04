#pragma once
#include "../world.h"
#include "../blockUpdater.h"

class SpruceSaplingUpdater : public BlockUpdater
{
public:
	SpruceSaplingUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};