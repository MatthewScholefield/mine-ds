#include "BaseMob.h"

bool canPlayerMobSpawnHere(WorldObject* world, int x, int y);

class PlayerMob : public BaseMob
{
public:
	/*int x;
	int y;
	int vy;
	int vx;
	bool alive;
	bool host; // Was this mob spawn'd by this nds?*/
	//bool potioneffects[5];
	bool deathScreen;
	int reheal;
	int tillBrightness;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	PlayerMob();
	PlayerMob(int x, int y);

	~ PlayerMob() { }
};
void playerMobInit();
