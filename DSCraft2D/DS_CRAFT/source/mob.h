#ifndef MOB_H
#include "player.h"
typedef struct{
	playerActor mobPlayer;
	int type;
	bool alive; //More like run the update function!
	int data[16];
}mobActor;

typedef struct{
	mobActor mobs[50];
}mobsStruct;
void mobUpdate(worldObject* world);
playerActor* mobSetup();
#define MOB_H
#endif
