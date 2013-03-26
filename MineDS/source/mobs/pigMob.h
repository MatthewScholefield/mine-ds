#include <stdio.h>
#include "../world.h"
#include "baseMob.h"
#ifndef PIG_MOB_H
#define PIG_MOB_H
bool canPigMobSpawnHere(worldObject* world,int x,int y);

class pigMob : public baseMob
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
	baseMob* target;
	int notarget;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
    	virtual void updateMob(worldObject* world);
       virtual void hurt(int amount,int type);	
	virtual bool isMyPlayer();
	pigMob();
   	pigMob(int x,int y);
};
void pigMobInit();
#endif
