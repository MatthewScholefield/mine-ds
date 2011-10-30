#ifndef player_h
#include <nds.h>
//A typedef struct is a structure and puts heaps of varibles in one place
#include "world.h"
typedef struct{
	int x;
	int y;
	int vy;
	int sx,sy;
	int health;
	bool facing_left;
	int frame; //for punching (next release)
	int blockx,blocky;
	int playerseed; //For multiplayer
	bool onblock;
	bool oldtop;
	bool person; //A mob can use the player's function's for gravity and stuff, I added this so that if a mob wants to use the players gravity functions it sets this to false.
}playerActor;
//Function definations 
void updateplayer(playerActor* player,worldObject* world);
void renderPlayer(playerActor* player,worldObject* world);
u16* playerGfx();
void playerFrame();
void playerCreateGfx();
void playerHurt(playerActor* player,int much,bool instant);
void test();
#define player_h
#endif
