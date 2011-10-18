//A block has Two Functions
void BEDROCK_render(int a, int b); //Render the block
void BEDROCK_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void BEDROCK_colision(playerActor* player,worldObject* world,int bx,int by,int result);
