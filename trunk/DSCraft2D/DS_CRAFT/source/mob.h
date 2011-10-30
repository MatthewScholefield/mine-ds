#ifndef MOB_H
#include "player.h"
typedef struct{
	playerActor mobPlayer;
}mobActor;
void mobUpdate(worldObject* world,playerActor* player);
void mobSetup();
#define MOB_H
#endif
