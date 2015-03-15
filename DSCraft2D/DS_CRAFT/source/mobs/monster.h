#ifndef monster_H
#include "../player.h"
#include "../world.h"
#include "../mob.h"
typedef struct{
	playerActor monsterPlayer;
	bool alive;
	int frames_till_jump;
}monsterActor;

void monsterUpdate(mobActor* monster,worldObject* world,playerActor* player);
void monster_setup();
#define monster_H
#endif
