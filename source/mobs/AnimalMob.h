#pragma once
#include <stdlib.h>
#include "BaseMob.h"
#include "../world.h"

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
	animal(AnimalType(rand() % 3)), scaredTimer(0), dir(true), mov(0) { }
	~ AnimalMob() { }
};
void animalMobInit();
bool canAnimalMobSpawnHere(WorldObject* world, int x, int y);
