#pragma once
#include "../world.h"
#include "../blockUpdater.h"

class OakSaplingUpdater : public BlockUpdater
{
public:
	OakSaplingUpdater();
	bool update(WorldObject* world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};

void saplingChanceUpdate(WorldObject *world, int x, int y, bool bg);
void saplingUpdate(WorldObject *world, int x, int y, bool bg);