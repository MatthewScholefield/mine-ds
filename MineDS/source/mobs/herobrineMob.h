#include "baseMob.h"
#ifndef HEROBRINE_MOB_H
#define HEROBRINE_MOB_H
bool canHerobrineMobSpawnHere(worldObject* world,int x,int y);

class herobrineMob : public baseMob
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
	herobrineMob();
   	herobrineMob(int x,int y);
};
void herobrineMobInit();
#endif
