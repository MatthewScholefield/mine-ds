#include "../player.h"
#include "../world.h"
//A block has Two Functions
void DIRT_render(int a, int b); //Render the block
void DIRT_setup(); //And copy the blocks graphics
void DIRT_colision(playerActor* player,worldObject* world,int bx,int by,int result);

