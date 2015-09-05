#pragma once
#include "../world.h"
#include "../blockUpdater.h"
#include "../graphics/graphics.h"

class FurnaceUpdater : public BlockUpdater
{
public:
	FurnaceUpdater();
	void alwaysUpdate(WorldObject* world, int x, int y, bool bg);
	Graphic fireParticle;
};

