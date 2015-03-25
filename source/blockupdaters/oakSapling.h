#pragma once
#include "../world.h"
#include "../blockupdate.h"

class oakSaplingUpdater : public blockUpdater {
public:
	oakSaplingUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};

void saplingChanceUpdate(worldObject *world, int x, int y, bool bg);
void saplingUpdate(worldObject *world, int x, int y, bool bg);