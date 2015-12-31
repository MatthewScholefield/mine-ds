#pragma once
#include <stdio.h>
#include "../world/World.h"
#include "BaseMob.h"
bool canMultiplayerMobSpawnHere(World &world, int x, int y);

class MultiplayerMob : public BaseMob
{
private:

	virtual int getMaxHealth()
	{
		return 20;
	}
public:
	void calcMiscData(World &world);
	void sendWifiUpdate();
	void updateMob(World &world);
	void hurt(int amount, int type);

	MultiplayerMob(int x, int y) : BaseMob(MOB_MULTIPLAYER, x, y, 6, 32, GraphicType::MOB_LARGE, 0) { }

	~MultiplayerMob() { }
};