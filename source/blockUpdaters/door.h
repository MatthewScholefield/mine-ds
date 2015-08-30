#pragma once
#include "../world.h"
#include "../blockUpdater.h"

class DoorUpdater : public BlockUpdater
{
public:
	DoorUpdater();
	bool update(WorldObject* world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};

class DoorTopOpenUpdater : public BlockUpdater
{
public:
	DoorTopOpenUpdater();
	bool update(WorldObject* world, int x, int y, bool bg);
};

class DoorTopClosedUpdater : public BlockUpdater
{
public:
	DoorTopClosedUpdater();
	bool update(WorldObject* world, int x, int y, bool bg);
};

class DoorBottomOpenUpdater : public BlockUpdater
{
public:
	DoorBottomOpenUpdater();
	bool update(WorldObject* world, int x, int y, bool bg);
};

class DoorBottomClosedUpdater : public BlockUpdater
{
public:
	DoorBottomClosedUpdater();
	bool update(WorldObject* world, int x, int y, bool bg);
};
