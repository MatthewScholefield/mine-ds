#pragma once
#include <stdio.h>
#include "../world/WorldObject.h"
#include "BaseMob.h"
bool canMultiplayerMobSpawnHere(WorldObject &world, int x, int y);

class MultiplayerMob : public BaseMob
{
private:

	virtual int getMaxHealth()
	{
		return 20;
	}
public:
	void calcMiscData(WorldObject &world);
	void saveToFile(FILE* sFile);
	void loadFromFile(FILE* sFile);
	void sendWifiUpdate();
	void updateMob(WorldObject &world);
	void hurt(int amount, int type);

	MultiplayerMob(int x, int y) : BaseMob(MOB_MULTIPLAYER, x, y, 6, 32, GRAPHIC_MOB, 0, 16, 32) { }

	~MultiplayerMob() { }
};