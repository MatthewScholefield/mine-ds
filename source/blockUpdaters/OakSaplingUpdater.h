#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class OakSaplingUpdater : public BlockUpdater
{
public:
	OakSaplingUpdater();
	bool update(World &world, int x, int y, bool bg);
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};

void saplingChanceUpdate(World &world, int x, int y, bool bg);
void saplingUpdate(World &world, int x, int y, bool bg);