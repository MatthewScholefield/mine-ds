//A block has Two Functions
void CACTUS_render(int a, int b); //Render the block
void CACTUS_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void CACTUS_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void CACTUS_update(int bx,int by,worldObject* world,playerActor* player);
