#include <stdio.h>
#include "../world.h"
#include "baseMob.h"
#ifndef ZOMBIE_MOB_H
#define ZOMBIE_MOB_H
bool canZombieMobSpawnHere(WorldObject* world, int x, int y);

class zombieMob : public baseMob {
public:
	/*int x;
	int y;
	int vy;
	int vx;
	bool alive;
	bool host; // Was this mob spawn'd by this nds?*/
	//bool potioneffects[5];
	int jump;
	baseMob* target;
	int notarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	zombieMob();
	zombieMob(int x, int y);

	~zombieMob() {
	}
};
void zombieMobInit();
#endif
