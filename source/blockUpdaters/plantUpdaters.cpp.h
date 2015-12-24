#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class ShrubUpdater : public BlockUpdater
{
public:
	ShrubUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};

class TallGrassUpdater : public BlockUpdater
{
public:
	TallGrassUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};

class RedFlowerUpdater : public BlockUpdater
{
public:
	RedFlowerUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};

class YellowFlowerUpdater : public BlockUpdater
{
public:
	YellowFlowerUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};

class RedMushroomUpdater : public BlockUpdater
{
public:
	RedMushroomUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};

class BrownMushroomUpdater : public BlockUpdater
{
public:
	BrownMushroomUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};