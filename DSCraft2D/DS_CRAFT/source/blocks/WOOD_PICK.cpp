#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* WOOD_PICKgfx;
void WOOD_PICK_render(int x,int y){
	createsprite32x32(x,y,WOOD_PICKgfx,false,0);	
}
void WOOD_PICK_setup(){
	WOOD_PICKgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*WOOD_PICK;
	dmaCopy(blocktiles,WOOD_PICKgfx,32*32);
}
