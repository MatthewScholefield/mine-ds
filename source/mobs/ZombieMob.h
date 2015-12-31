#pragma once
#include <stdio.h>
#include "../world/WorldObject.h"
#include "BaseMob.h"
#define ZOMBIE_SPEED 0.5

bool canZombieMobSpawnHere(World &world, int x, int y);

class ZombieMob : public BaseMob
{
public:
	void calcMiscData(World &world);
	void sendWifiUpdate();
	void updateMob(World &world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	ZombieMob(int x, int y) : BaseMob(MOB_ZOMBIE, x, y, 6, 32, GraphicType::MOB_LARGE, 3) { }

	~ZombieMob() { }
};