#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* REDSTONE_OREgfx;
void REDSTONE_ORE_render(int x,int y){
	createsprite32x32(x,y,REDSTONE_OREgfx,false,0);	
}
void REDSTONE_ORE_setup(){
	REDSTONE_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*REDSTONE_ORE;
	dmaCopy(blocktiles,REDSTONE_OREgfx,32*32);
}
