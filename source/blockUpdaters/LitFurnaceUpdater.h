#pragma once
#include "../world.h"
#include "blockUpdater.h"
#include "../graphics/graphics.h"
#include "../graphics/particleHandler.h"

class LitFurnaceUpdater : public BlockUpdater
{
private:
	int bufferIndex;
	Graphic fireParticle;
	Graphic clones[16];
public:
	LitFurnaceUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};

