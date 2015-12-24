#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class CactusUpdater : public BlockUpdater
{
public:
	CactusUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};
