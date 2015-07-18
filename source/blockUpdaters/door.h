#pragma once
#include "../world.h"
#include "../blockUpdater.h"

class DoorUpdater : public BlockUpdater
{
public:
	DoorUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};

class DoorTopOpenUpdater : public BlockUpdater
{
public:
	DoorTopOpenUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class DoorTopClosedUpdater : public BlockUpdater
{
public:
	DoorTopClosedUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class DoorBottomOpenUpdater : public BlockUpdater
{
public:
	DoorBottomOpenUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};

class DoorBottomClosedUpdater : public BlockUpdater
{
public:
	DoorBottomClosedUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};
