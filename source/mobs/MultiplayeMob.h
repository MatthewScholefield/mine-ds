#pragma once
#include <stdio.h>
#include "../world.h"
#include "BaseMob.h"
bool canMultiplayerMobSpawnHere(WorldObject* world, int x, int y);

class MultiplayerMob : public BaseMob
{
public:
	/*int x;
	int y;
	int vy;
	int vx;
	bool alive;
	bool host; // Was this mob spawn'd by this nds?*/
	//bool potioneffects[5];
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	MultiplayerMob();
	MultiplayerMob(int x, int y);

	~ MultiplayerMob() { }
};
void multiplayerMobInit();