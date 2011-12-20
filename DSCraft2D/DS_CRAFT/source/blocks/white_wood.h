//A block has Two Functions
void WHITE_WOOD_render(int a, int b); //Render the block
void WHITE_WOOD_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void WHITE_WOOD_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void WHITE_WOOD_mine(worldObject* world,int* mine_time,int x,int y);
