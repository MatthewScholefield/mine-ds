#pragma once
#include <stdio.h>
#include "../world.h"
#include "BaseMob.h"
#define ZOMBIE_SPEED 0.5

bool canZombieMobSpawnHere(WorldObject* world, int x, int y);

class ZombieMob : public BaseMob
{
public:
	/*int x;
	int y;
	int vy;
	int vx;
	bool alive;
	bool host; // Was this mob spawn'd by this nds?*/
	//bool potioneffects[5];
	int jump;
	BaseMob* target;
	int noTarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	ZombieMob();
	ZombieMob(int x, int y);

	~ ZombieMob() { }
};
void zombieMobInit();