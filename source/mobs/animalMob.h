#pragma once
#include "baseMob.h"
#include "../world.h"

enum AnimalType {
	ANIMAL_COW = 0,
	ANIMAL_SHEEP = 1,
	ANIMAL_PIG = 2
};

class animalMob : public baseMob {
public:
	int scaredTimer; //Limits how long the cow is scared
	bool dir; //Direction of cow movement
	int mov; //Loop counter for cow movement
	baseMob* target;
	int notarget;
	AnimalType animal;

	virtual void saveToFile(FILE* sFile);
	virtual void loadFromFile(FILE* sFile);
	virtual void sendWifiUpdate();
	virtual void updateMob(WorldObject* world);
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	animalMob();

	animalMob(int x, int y);

	~animalMob() {}
};
void animalMobInit();
bool canAnimalMobSpawnHere(WorldObject* world, int x, int y);
