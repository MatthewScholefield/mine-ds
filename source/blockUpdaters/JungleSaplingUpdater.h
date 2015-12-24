#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class JungleSaplingUpdater : public BlockUpdater
{
public:
	JungleSaplingUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};