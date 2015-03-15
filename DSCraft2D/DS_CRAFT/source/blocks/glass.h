//A block has Two Functions
void GLASS_render(int a, int b); //Render the block
void GLASS_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void GLASS_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void GLASS_mine(worldObject* world,int* mine_time,int x,int y);
