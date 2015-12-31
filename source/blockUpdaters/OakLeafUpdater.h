#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

void leafChanceUpdateGeneric(World &world, int x, int y, bool bg);

class OakLeafUpdater : public BlockUpdater
{
public:
	OakLeafUpdater();
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};