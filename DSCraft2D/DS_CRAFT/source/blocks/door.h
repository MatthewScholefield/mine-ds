#include "../player.h"
#include "../world.h"
//A block has Two Functions
void DOOR_render(int a, int b); //Render the block
void DOOR_setup(); //And copy the blocks graphics
void DOOR_update(int bx,int by,worldObject* world,playerActor* player);
