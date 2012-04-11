#include <stdio.h>
class baseMob
{
    public:
	int x;
	int y;
	int vy;
	int vx;
    //bool potioneffects[5];
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	void setXYPos(int x,int y);
    void resetVelocity();	
    virtual void updateMob();	
	baseMob();
    baseMob(int x,int y);
};
