#pragma once
#include "BaseMob.h"
#include "../graphics/graphics.h"

class ItemMob : public BaseMob
{
public:
	int blockID;
	int displayID;
	int amount;
	int floatY;
	int hurtStage;
	int brightness;
	Graphic *itemGraphic;
	//bool loadedBlockGfx;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	virtual void killMob();
	ItemMob(int x, int y);

	~ ItemMob() { }
};
