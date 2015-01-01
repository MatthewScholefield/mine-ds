#include <stdio.h>
#include "../world.h"
#include "baseMob.h"
#ifndef SHEEP_MOB_H
#define SHEEP_MOB_H
bool canSheepMobSpawnHere(worldObject* world,int x,int y);

class sheepMob : public baseMob
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
	baseMob* target;
	int notarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(worldObject* world);
	virtual void hurt(int amount,int type);
	virtual bool isMyPlayer();
	sheepMob();
	sheepMob(int x,int y);
};
void sheepMobInit();
#endif
