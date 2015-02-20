#pragma once
#include "../world.h"
#include "../blockupdate.h"


class junglegrassUpdater : public blockUpdater
{
  public:
    junglegrassUpdater();
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};

