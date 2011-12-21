//A block has Two Functions
void SAND_render(int a, int b,int c,int d,worldObject* e); //Render the block
void SAND_setup(); //And copy the blocks graphics
#include "../player.h"
#include "../world.h"
void SAND_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void SAND_mine(worldObject* world,int* mine_time,int x,int y);
void SAND_render_nofall(int x,int y);
void SAND_update(int bx,int by,worldObject* world,playerActor* player);
