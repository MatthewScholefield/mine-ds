#pragma once
#include <stdlib.h>
#include "BaseMob.h"
bool canHerobrineMobSpawnHere(WorldObject &world, int x, int y);

class HerobrineMob : public BaseMob
{
private:

	virtual int getMaxHealth()
	{
		return 7;
	}
public:
	int waitingCount;

	void calcMiscData(WorldObject &world);
	void saveToFile(FILE* sFile);
	void loadFromFile(FILE* sFile);
	void sendWifiUpdate();
	void updateMob(WorldObject &world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	HerobrineMob(int x, int y) : BaseMob(MOB_HEROBRINE, x, y, 6, 32, GRAPHIC_MOB, 8, 16, 32),
	waitingCount(-rand() % 4000) { }

	~HerobrineMob() { }
};