#pragma once
#include <stdlib.h>
#include "BaseMob.h"
bool canHerobrineMobSpawnHere(World &world, int x, int y);

class HerobrineMob : public BaseMob
{
private:

	virtual int getMaxHealth()
	{
		return 7;
	}
public:
	int waitingCount;

	void calcMiscData(World &world);
	void sendWifiUpdate();
	void updateMob(World &world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	HerobrineMob(int x, int y) : BaseMob(MOB_HEROBRINE, x, y, 6, 32, GraphicType::MOB_LARGE, 8),
	waitingCount(-rand() % 4000) { }

	~HerobrineMob() { }
};