//A block has Two Functions
void DIAMOND_ORE_render(int a, int b); //Render the block
void DIAMOND_ORE_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void DIAMOND_ORE_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void DIAMOND_ORE_mine(worldObject* world,int* mine_time,int x,int y);
