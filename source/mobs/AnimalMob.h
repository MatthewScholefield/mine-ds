#pragma once
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
	int scaredTimer; //Limits how long the cow is scared
	bool dir; //Direction of cow movement
	int mov; //Loop counter for cow movement
	BaseMob* target;
	int notarget;
	AnimalType animal;

	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	AnimalMob();

	AnimalMob(int x, int y);

	~ AnimalMob() { }
};
void animalMobInit();
bool canAnimalMobSpawnHere(WorldObject* world, int x, int y);
