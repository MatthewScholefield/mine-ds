#include "../player.h"
#include "../world.h"
//A block has Two Functions
void STONE_render(int a, int b); //Render the block
void STONE_setup(); //And copy the blocks graphics
void STONE_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void STONE_mine(worldObject* world,int* mine_time,int x,int y);

