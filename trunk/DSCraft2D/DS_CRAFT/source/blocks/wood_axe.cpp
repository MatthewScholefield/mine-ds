#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* WOOD_AXEgfx;
void WOOD_AXE_render(int x,int y){
	createsprite32x32(x,y,WOOD_AXEgfx,false,0);	
}
void WOOD_AXE_setup(){
	WOOD_AXEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*WOOD_AXE;
	dmaCopy(blocktiles,WOOD_AXEgfx,32*32);
}
