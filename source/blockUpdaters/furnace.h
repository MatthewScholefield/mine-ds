#pragma once
#include "../world.h"
#include "../blockUpdater.h"
#include "../graphics/graphics.h"

class FurnaceUpdater : public BlockUpdater
{
private:
	int bufferIndex = 0;
public:
	FurnaceUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
	Graphic fireParticle;
	Graphic clones[16];
};

