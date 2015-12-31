#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class DoorUpdater : public BlockUpdater
{
public:
	DoorUpdater();
	bool update(World &world, int x, int y, bool bg);
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};

class DoorTopOpenUpdater : public BlockUpdater
{
public:
	DoorTopOpenUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class DoorTopClosedUpdater : public BlockUpdater
{
public:
	DoorTopClosedUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class DoorBottomOpenUpdater : public BlockUpdater
{
public:
	DoorBottomOpenUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class DoorBottomClosedUpdater : public BlockUpdater
{
public:
	DoorBottomClosedUpdater();
	bool update(World &world, int x, int y, bool bg);
};
