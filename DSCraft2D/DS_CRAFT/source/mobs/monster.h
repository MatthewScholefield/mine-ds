#ifndef monster_H
#include "../player.h"
#include "../world.h"
typedef struct{
	playerActor monsterPlayer;
	bool alive;
	int frames_till_jump;
}monsterActor;

void monsterUpdate(monsterActor* monster,worldObject* world,playerActor* player);
#define monster_H
#endif
