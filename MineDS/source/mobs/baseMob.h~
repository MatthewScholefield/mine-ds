#include <stdio.h>
#include "../world.h"
#ifndef BASE_MOB_H
#define BASE_MOB_H
bool canBaseMobSpawnHere(worldObject* world,int x,int y);
class baseMob
{
    public:
	int x;
	int y;
	int vy;
	int vx;
	int sx;
	int sy;
	int gravityValue;
	int gravity;
	bool onground;
	bool alive;
	int animation;
	bool host; // Was this mob spawn'd by this nds?
	bool colisions[5];
    //bool potioneffects[5];
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	void killMob();
	void unKillMob();
	void setXYPos(int x,int y);
    void resetVelocity();	
    virtual void updateMob(worldObject* world);	
	baseMob();
    baseMob(int x,int y);
};
void baseMobInit();
#endif
