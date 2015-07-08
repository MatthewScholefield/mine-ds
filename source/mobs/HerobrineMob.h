#pragma once
#include "BaseMob.h"
bool canHerobrineMobSpawnHere(WorldObject* world, int x, int y);

class HerobrineMob : public BaseMob
{
public:
	int waitingCount;
	int jump;
	int notarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	HerobrineMob();
	HerobrineMob(int x, int y);

	~ HerobrineMob() { }
};
void herobrineMobInit();