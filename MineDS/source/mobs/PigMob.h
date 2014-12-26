#pragma once
#include <stdio.h>
#include "../world.h"
#include "BaseMob.h"
bool canPigMobSpawnHere(WorldObject* world,int x,int y);

class PigMob : public BaseMob
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
	bool scared=false; //Whether pig is scared and will run away from the player
	int scaredtimer; //Limits how long the pig is scared
	bool dir; //Direction of pig movement
	int mov; //Loop counter for pig movement
	BaseMob* target;
	int notarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount,int type);
	virtual bool isMyPlayer();
	PigMob();
	PigMob(int x,int y);
};
void pigMobInit();