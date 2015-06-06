#pragma once
#include "baseMob.h"
#include "../graphics/graphics.h"

class itemMob : public baseMob {
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
	itemMob(int x, int y);

	~itemMob() {
	}
};
