#ifndef player_h
#include <nds.h>
#include "world.h"
//A typedef struct is a structure and puts heaps of varibles in one place
typedef struct{
	int x;
	int y;
	int vy;
	int health;
	bool handout; //for punching (next release)
	int blockx,blocky;
	int playerseed; //For multiplayer
	bool onblock;
	
}playerActor;
//Function definations 
void updateplayer(playerActor* player,worldObject* world);
u16* playerGfx();
void playerCreateGfx();
#define player_h
#endif
