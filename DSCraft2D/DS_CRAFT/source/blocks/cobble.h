//A block has Two Functions
void COBBLE_render(int a, int b); //Render the block
void COBBLE_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void COBBLE_colision(playerActor* player,worldObject* world,int bx,int by,int result);
