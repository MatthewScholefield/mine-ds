//A block has Two Functions
void SAND_render(int a, int b); //Render the block
void SAND_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void SAND_colision(playerActor* player,worldObject* world,int bx,int by,int result);
