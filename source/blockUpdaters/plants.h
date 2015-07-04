#pragma once
#include "../world.h"
#include "../blockUpdater.h"

class ShrubUpdater : public BlockUpdater
{
public:
	ShrubUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class TallGrassUpdater : public BlockUpdater
{
public:
	TallGrassUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class RedFlowerUpdater : public BlockUpdater
{
public:
	RedFlowerUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class YellowFlowerUpdater : public BlockUpdater
{
public:
	YellowFlowerUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class RedMushroomUpdater : public BlockUpdater
{
public:
	RedMushroomUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class BrownMushroomUpdater : public BlockUpdater
{
public:
	BrownMushroomUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};