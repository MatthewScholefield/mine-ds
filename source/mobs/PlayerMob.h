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
	, mineSprite(GraphicType::MOB_ANIM, 2), deathScreen(false), tillBrightness(0)
	{
		normalSprite.reload(GraphicType::MOB_ANIM, 0);
		hurtSprite.reload(GraphicType::MOB_LARGE, 1);
		controlsEnabled = true;
		addInventory(CHEST);
		addInventory(DIRT, 27);
	}

	~PlayerMob()
	{
	}
};
void playerMobInit();
