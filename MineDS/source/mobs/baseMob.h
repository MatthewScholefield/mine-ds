#include <stdio.h>
#include "../world.h"
#ifndef BASE_MOB_H
#define BASE_MOB_H
//
bool canBaseMobSpawnHere(worldObject* world, int x, int y);

class baseMob {
public:
	float x;
	int y;
	float vy;
	float vx;
	int sx;
	int sy;
	bool smallmob = false;
	int gravityValue;
	int gravity;
	bool onground;
	bool alive;
	int animation;
	int animationclearframes;
	int mobtype;
	int timeOnCactus;
	bool onCactus;
	int health; //Health of the mob, 0 is dead ;)
	int timeTillWifiUpdate;
	int ping;
	int mobId; // Needed for chasing mobs
	bool host; // Was this mob spawn'd by this nds?
	bool collisions[5];
	//bool potioneffects[5];
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	bool facing;
	void killMob();
	void unKillMob();
	void setXYPos(int x, int y);
	void resetVelocity();
	virtual void updateMob(worldObject* world);
	baseMob();
	baseMob(int x, int y);
};
void baseMobInit();
#endif
