#include "../world.h"
#include "../player.h"
void LEAVES_render(int a, int b);
void LEAVES_setup();
void LEAVES_colision(playerActor* player,worldObject* world,int bx,int by,int result); //And copy the blocks graphics
void LEAVES_mine(worldObject* world,int* mine_time,int x,int y);


void LEAVES_update(int bx,int by,worldObject* world,playerActor* player);
