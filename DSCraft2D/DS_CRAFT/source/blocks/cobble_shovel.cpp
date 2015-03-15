#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* COBBLE_SHOVELgfx;
void COBBLE_SHOVEL_render(int x,int y){
	createsprite32x32(x,y,COBBLE_SHOVELgfx,false,0);	
}
void COBBLE_SHOVEL_setup(){
	COBBLE_SHOVELgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COBBLE_SHOVEL;
	dmaCopy(blocktiles,COBBLE_SHOVELgfx,32*32);
}
