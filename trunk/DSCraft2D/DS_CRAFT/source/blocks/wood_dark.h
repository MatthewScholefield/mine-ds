//A block has Two Functions
void DARK_WOOD_render(int a, int b); //Render the block
void DARK_WOOD_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void DARK_WOOD_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void DARK_WOOD_mine(worldObject* world,int* mine_time,int x,int y);
