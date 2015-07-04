#pragma once
#include "../world.h"
#include "../blockupdate.h"
#include "../graphics/graphics.h"

class FurnaceUpdater : public BlockUpdater
{
public:
	FurnaceUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
	Graphic fireParticle;
};

