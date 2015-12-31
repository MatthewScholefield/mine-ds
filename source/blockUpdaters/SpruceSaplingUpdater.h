#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class SpruceSaplingUpdater : public BlockUpdater
{
public:
	SpruceSaplingUpdater();
	bool update(World &world, int x, int y, bool bg);
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};