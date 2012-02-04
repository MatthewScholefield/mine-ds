#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* COBBLE_AXEgfx;
void COBBLE_AXE_render(int x,int y){
	createsprite32x32(x,y,COBBLE_AXEgfx,false,0);	
}
void COBBLE_AXE_setup(){
	COBBLE_AXEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COBBLE_AXE;
	dmaCopy(blocktiles,COBBLE_AXEgfx,32*32);
}
