#pragma once
#include "BaseMob.h"
#include <cstdlib>
#include "../graphics/graphics.h"

class ItemMob : public BaseMob
{
public:
	int blockID;
	int displayID;
	int amount;
	int floatY;
	int brightness;
	Graphic *itemGraphic;
	//bool loadedBlockGfx;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	virtual void kill();

	ItemMob(int x, int y, int blockID, int amount, int displayID, float vx) : BaseMob(), blockID(blockID), displayID(displayID), amount(amount)
	{
		this->x = x;
		this->y = y;

		sx = 8;
		sy = 8;
		health = 1;
		type = MOB_ITEM;
		floatY = 0;
		if (vx == 54321.0)
		{
			//Set initial velocity
			vx = double((rand() % 10) + 40) / 100.0;
			//Set direction
			vx *= (rand() % 2) ? - 1 : 1;
		}
		this->vx = vx;
		vy = 0;
		itemGraphic = NULL;
		brightness = 0;
	}

	~ ItemMob() { }
};
