//A block has Two Functions
void IRON_ORE_render(int a, int b); //Render the block
void IRON_ORE_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void IRON_ORE_colision(playerActor* player,worldObject* world,int bx,int by,int result);

void IRON_ORE_mine(worldObject* world,int* mine_time,int x,int y);
