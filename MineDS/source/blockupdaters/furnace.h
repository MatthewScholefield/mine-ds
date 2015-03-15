#pragma once
#include "../world.h"
#include "../blockupdate.h"
#include "../graphics/graphics.h"

class furnaceUpdater : public blockUpdater {
public:
	furnaceUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
	Graphic fireParticle;
};

