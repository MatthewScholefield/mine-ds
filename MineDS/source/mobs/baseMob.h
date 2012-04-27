#include <stdio.h>
#include "../world.h"
bool canBaseMobSpawnHere(worldObject* world,int x,int y);

class baseMob
{
    public:
	int x;
	int y;
	int vy;
	int vx;
	bool alive;
	bool host; // Was this mob spawn'd by this nds?
    //bool potioneffects[5];
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	void setXYPos(int x,int y);
    void resetVelocity();	
    virtual void updateMob(worldObject* world);	
	baseMob();
    baseMob(int x,int y);
};
void baseMobInit();
