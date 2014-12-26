#pragma once
#include <stdio.h>
#include "../world.h"
#include "BaseMob.h"
bool canCowMobSpawnHere(WorldObject* world,int x,int y);

class CowMob : public BaseMob
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
	bool scared=false; //Whether cow is scared and will run away from the player
	int scaredtimer; //Limits how long the cow is scared
	bool dir; //Direction of cow movement
	int mov; //Loop counter for cow movement
	BaseMob* target;
	int notarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount,int type);
	virtual bool isMyPlayer();
	CowMob();
	CowMob(int x,int y);
};
void cowMobInit();