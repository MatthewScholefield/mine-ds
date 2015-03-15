#pragma once
#include "../world.h"
#include "../blockupdate.h"

class shrubUpdater : public blockUpdater {
public:
	shrubUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
};

class tallGrassUpdater : public blockUpdater {
public:
	tallGrassUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
};

class redFlowerUpdater : public blockUpdater {
public:
	redFlowerUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
};

class yellowFlowerUpdater : public blockUpdater {
public:
	yellowFlowerUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
};

class redMushroomUpdater : public blockUpdater {
public:
	redMushroomUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
};

class brownMushroomUpdater : public blockUpdater {
public:
	brownMushroomUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
};