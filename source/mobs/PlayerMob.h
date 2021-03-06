#pragma once
#include "BaseMob.h"
#include "../blockID.h"
bool canPlayerMobSpawnHere(World &world, int x, int y);

class PlayerMob : public BaseMob
{
private:
	static Graphic mineSprite, fullHeart, halfHeart;
	static bool controlsEnabled;

	static void showHealth(int health);
	virtual int getMaxHealth()
	{
		return 20;
	}
public:
	bool deathScreen;
	int tillBrightness;

	static void setControlsEnabled(bool enabled);
	void calcMiscData(World &world);
	void saveToFile(FILE* sFile);
	void loadFromFile(FILE* sFile);
	void sendWifiUpdate();
	void updateMob(World &world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	PlayerMob(int x, int y) : BaseMob(MOB_PLAYER, x, y, 6, 32)
	, deathScreen(false), tillBrightness(0)
	{
		normalSprite.reload(GraphicType::MOB_ANIM, 0);
		hurtSprite.reload(GraphicType::MOB_LARGE, 1);
		controlsEnabled = true;
	}

	~PlayerMob()
	{
	}
};
void playerMobInit();
