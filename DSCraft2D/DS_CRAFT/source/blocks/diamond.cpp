#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* DIAMONDgfx;
void DIAMOND_render(int x,int y){
	createsprite32x32(x,y,DIAMONDgfx,false,0);	
}
void DIAMOND_setup(){
	DIAMONDgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DIAMOND;
	dmaCopy(blocktiles,DIAMONDgfx,32*32);
}
