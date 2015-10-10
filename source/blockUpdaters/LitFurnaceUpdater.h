#pragma once
#include "../world.h"
#include "blockUpdater.h"
#include "../graphics/graphics.h"
#include "../graphics/particleHandler.h"

class LitFurnaceUpdater : public BlockUpdater
{
private:
	static int bufferIndex; // is there a reason this wasn't static before? ~Cool As
public:
	LitFurnaceUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
	Graphic fireParticle;
	Graphic clones[16];
};

