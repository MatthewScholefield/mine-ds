//A block has Two Functions
void SNOW_GRASS_render(int a, int b); //Render the block
void SNOW_GRASS_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void SNOW_GRASS_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void SNOW_GRASS_update(int bx,int by,worldObject* world,playerActor* player);
