#include <stdio.h>
#include "../world.h"
#include "baseMob.h"
bool canMplayerMobSpawnHere(worldObject* world, int x, int y);

class MplayerMob : public baseMob {
public:
	/*int x;
	int y;
	int vy;
	int vx;
	bool alive;
	bool host; // Was this mob spawn'd by this nds?*/
	//bool potioneffects[5];
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(worldObject* world);
	virtual void hurt(int amount, int type);
	MplayerMob();
	MplayerMob(int x, int y);

	~MplayerMob() {
	}
};
void MplayerMobInit();
