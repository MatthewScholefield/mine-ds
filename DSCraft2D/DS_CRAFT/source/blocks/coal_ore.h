#include "../player.h"
#include "../world.h"
void COAL_ORE_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void COAL_ORE_render(int a, int b); //Render the block
void COAL_ORE_setup(); //And copy the blocks graphics
void COAL_ORE_mine(worldObject* world,int* mine_time,int x,int y);
