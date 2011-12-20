//A block has Two Functions
void PLANKS_render(int a, int b); //Render the block
void PLANKS_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void PLANKS_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void PLANKS_mine(worldObject* world,int* mine_time,int x,int y);
