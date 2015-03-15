#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* COBBLE_PICKAXEgfx;
void COBBLE_PICKAXE_render(int x,int y){
	createsprite32x32(x,y,COBBLE_PICKAXEgfx,false,0);	
}
void COBBLE_PICKAXE_setup(){
	COBBLE_PICKAXEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COBBLE_PICKAXE;
	dmaCopy(blocktiles,COBBLE_PICKAXEgfx,32*32);
}
