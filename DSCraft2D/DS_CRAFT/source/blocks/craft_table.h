//A block has Two Functions
void CRAFT_TABLE_render(int a, int b); //Render the block
void CRAFT_TABLE_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void CRAFT_TABLE_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void CRAFT_TABLE_mine(worldObject* world,int* mine_time,int x,int y);
