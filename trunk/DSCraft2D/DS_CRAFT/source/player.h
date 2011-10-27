#ifndef player_h
#include <nds.h>
//A typedef struct is a structure and puts heaps of varibles in one place
#include "world.h"
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
void playerHurt(playerActor* player,int much,bool instant);
void test();
#define player_h
#endif
