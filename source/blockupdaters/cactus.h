#pragma once
#include "../world.h"
#include "../blockupdate.h"

class cactusUpdater : public blockUpdater {
public:
	cactusUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};