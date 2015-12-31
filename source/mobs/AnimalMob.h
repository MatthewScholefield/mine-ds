#pragma once
#include <stdlib.h>
#include "BaseMob.h"
#include "../world/WorldObject.h"
#include "../graphics/graphics.h"

enum AnimalType
{
	ANIMAL_PIG = 0,
	ANIMAL_COW = 1,
	ANIMAL_SHEEP = 2
};

class AnimalMob : public BaseMob
{
private:
	static const int FRAME[3];
public:
	AnimalType animal;
	int scaredTimer; //Limits how long the cow is scared
	bool dir; //Direction of cow movement
	int mov; //Loop counter for cow movement

	void calcMiscData(World &world);
	void sendWifiUpdate();
	void updateMob(World &world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	AnimalMob(int x, int y) : BaseMob(MOB_ANIMAL, x, y, 10, 16),
	animal(AnimalType(rand() % 3)), scaredTimer(0), dir(true), mov(0)
	{
		normalSprite.reload(GraphicType::MOB_SMALL, FRAME[animal]);
		hurtSprite.reload(GraphicType::MOB_SMALL, FRAME[animal] + 1);
	}

	~AnimalMob() { }
};
bool canAnimalMobSpawnHere(World &world, int x, int y);
