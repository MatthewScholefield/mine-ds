//A block has Two Functions
void LOG_render(int a, int b); //Render the block
void LOG_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void LOG_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void LOG_mine(worldObject* world,int* mine_time,int x,int y);
