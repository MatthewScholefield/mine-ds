#include "../player.h"
#include "../world.h"
void TORCH_render(int a, int b);
void TORCH_setup();
void TORCH_update(int bx,int by,worldObject* world,playerActor* player);
void TORCH_mine(worldObject* world,int* mine_time,int x,int y);
