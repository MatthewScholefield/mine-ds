#pragma once
#include <stdio.h>
#include "../world.h"
#include "BaseMob.h"
bool canSheepMobSpawnHere(WorldObject* world,int x,int y);

class SheepMob : public BaseMob
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
	bool scared=false; //Whether sheep is scared and will run away from the player
	int scaredtimer; //Limits how long the sheep is scared
	bool dir; //Direction of sheep movement
	int mov; //Loop counter for sheep movement
	BaseMob* target;
	int notarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount,int type);
	virtual bool isMyPlayer();
	SheepMob();
	SheepMob(int x,int y);
};
void sheepMobInit();