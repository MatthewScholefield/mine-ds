#pragma once
#include <stdio.h>
#include "../world.h"

#define SIDE_BOTTOM 0
#define SIDE_RIGHT 1
#define SIDE_LEFT 2
#define SIDE_TOP 3
#define COLLISION_STUCK 4

bool canBaseMobSpawnHere(WorldObject* world, int x, int y);

class baseMob {
public:
	float x;
	float y;
	float vy;
	float vx;
	int sx;
	int sy;
	bool smallmob = false;
	bool alive;
	int animation;
	int animationClearFrames;
	int mobType;
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
	virtual void updateMob(WorldObject* world);
	baseMob();
	baseMob(int x, int y);

	virtual ~baseMob() {
	}
};
void baseMobInit();
bool jumpHurtType(int hurtType);