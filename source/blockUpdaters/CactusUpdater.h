#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class CactusUpdater : public BlockUpdater
{
public:
	CactusUpdater();
	bool update(World &world, int x, int y, bool bg);
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};
