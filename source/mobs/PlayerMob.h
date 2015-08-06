#pragma once
#include "BaseMob.h"

bool canPlayerMobSpawnHere(WorldObject* world, int x, int y);

class PlayerMob : public BaseMob
{
private:
	Graphic mineSprite;
	virtual int getMaxHealth()
	{
		return 20;
	}
public:
	bool deathScreen;
	int tillBrightness;
  bool initGraphics;
	void calcMiscData(WorldObject* world);
	void saveToFile(FILE* sFile);
	void loadFromFile(FILE* sFile);
	void sendWifiUpdate();
	void updateMob(WorldObject* world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	PlayerMob(int x, int y) : BaseMob(MOB_PLAYER, x, y, 6, 32)
	, mineSprite(), deathScreen(false), tillBrightness(0), initGraphics(false)  { }

	~PlayerMob()
	{
		unloadGraphic(&mineSprite);
	}
};
void playerMobInit();
