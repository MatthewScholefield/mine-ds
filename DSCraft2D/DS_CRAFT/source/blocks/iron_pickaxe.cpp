#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* IRON_PICKAXEgfx;
void IRON_PICKAXE_render(int x,int y){
	createsprite32x32(x,y,IRON_PICKAXEgfx,false,0);	
}
void IRON_PICKAXE_setup(){
	IRON_PICKAXEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*IRON_PICKAXE;
	dmaCopy(blocktiles,IRON_PICKAXEgfx,32*32);
}
