#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class JungleSaplingUpdater : public BlockUpdater
{
public:
	JungleSaplingUpdater();
	bool update(World &world, int x, int y, bool bg);
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};