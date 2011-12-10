//A block has Two Functions
void GRASS_render(int a, int b); //Render the block
void GRASS_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void GRASS_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void GRASS_update(int bx,int by,worldObject* world,playerActor* player);
void GRASS_mine(worldObject* world,int* mine_time,int x,int y);
