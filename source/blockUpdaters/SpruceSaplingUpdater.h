#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class SpruceSaplingUpdater : public BlockUpdater
{
public:
	SpruceSaplingUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};