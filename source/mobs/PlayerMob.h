#pragma once
#include "BaseMob.h"
#include "../blockID.h"
bool canPlayerMobSpawnHere(WorldObject &world, int x, int y);

class PlayerMob : public BaseMob
{
private:
	Graphic mineSprite;
	static bool controlsEnabled;
	virtual int getMaxHealth()
	{
		return 20;
	}
public:
	bool deathScreen;
	int tillBrightness;

	static void setControlsEnabled(bool enabled);
	void calcMiscData(WorldObject &world);
	void saveToFile(FILE* sFile);
	void loadFromFile(FILE* sFile);
	void sendWifiUpdate();
	void updateMob(WorldObject &world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	PlayerMob(int x, int y) : BaseMob(MOB_PLAYER, x, y, 6, 32)
	, mineSprite(GRAPHIC_MOB_ANIM, 2, 16, 32), deathScreen(false), tillBrightness(0)
	{
		normalSprite = Graphic(GRAPHIC_MOB_ANIM, 0, 16, 32);
		hurtSprite = Graphic(GRAPHIC_MOB, 0, 16, 32);
		controlsEnabled = true;
		addInventory(CHEST);
		addInventory(DIRT, 27);
	}

	~PlayerMob()
	{
	}
};
void playerMobInit();
