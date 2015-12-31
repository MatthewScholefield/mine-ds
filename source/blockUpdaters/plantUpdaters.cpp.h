#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class ShrubUpdater : public BlockUpdater
{
public:
	ShrubUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class TallGrassUpdater : public BlockUpdater
{
public:
	TallGrassUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class RedFlowerUpdater : public BlockUpdater
{
public:
	RedFlowerUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class YellowFlowerUpdater : public BlockUpdater
{
public:
	YellowFlowerUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class RedMushroomUpdater : public BlockUpdater
{
public:
	RedMushroomUpdater();
	bool update(World &world, int x, int y, bool bg);
};

class BrownMushroomUpdater : public BlockUpdater
{
public:
	BrownMushroomUpdater();
	bool update(World &world, int x, int y, bool bg);
};