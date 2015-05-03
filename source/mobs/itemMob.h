#pragma once
#include "baseMob.h"
#include "../graphics/graphics.h"

class itemMob : public baseMob {
public:
	Graphic itemGraphic;
	int blockID;
	int displayID;
	int amount;
	int floatY;
	bool loadedBlockGfx;
	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(worldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	virtual void killMob();
	itemMob(int x, int y);

	~itemMob() {
	}
};
