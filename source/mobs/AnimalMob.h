#pragma once
#include <stdlib.h>
#include "BaseMob.h"
#include "../world.h"
#include "../graphics/graphics.h"

enum AnimalType
{
	ANIMAL_COW = 0,
	ANIMAL_SHEEP = 1,
	ANIMAL_PIG = 2
};

class AnimalMob : public BaseMob
{
public:
	AnimalType animal;
	int scaredTimer; //Limits how long the cow is scared
	bool dir; //Direction of cow movement
	int mov; //Loop counter for cow movement

	void calcMiscData(WorldObject *world);
	void saveToFile(FILE* sFile);
	void loadFromFile(FILE* sFile);
	void sendWifiUpdate();
	void updateMob(WorldObject* world);
	void hurt(int amount, int type);
	bool isMyPlayer();

	AnimalMob(int x, int y) : BaseMob(MOB_ANIMAL, x, y, 10, 16),
	animal(AnimalType(rand() % 3)), scaredTimer(0), dir(true), mov(0)
	{
		int frame = 0;
		switch (animal)
		{
			case ANIMAL_PIG:
				frame = 10;
				break;
			case ANIMAL_COW:
				frame = 12;
				break;
			case ANIMAL_SHEEP:
				frame = 14;
				break;
		}
		loadGraphic(&normalSprite, GRAPHIC_MOB, frame, 16, 16);
		loadGraphic(&hurtSprite, GRAPHIC_MOB, frame + 1, 16, 16);
		used = true;
	}
	~ AnimalMob() { }
};
void animalMobInit();
bool canAnimalMobSpawnHere(WorldObject* world, int x, int y);
