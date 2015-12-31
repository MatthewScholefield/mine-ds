#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"
#include "../graphics/graphics.h"
#include "../graphics/particleHandler.h"

class LitFurnaceUpdater : public BlockUpdater
{
private:
	static Graphic fireParticle;
public:
	LitFurnaceUpdater();
	bool update(World &world, int x, int y, bool bg);
};

