#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* REDSTONEgfx;
void REDSTONE_render(int x,int y){
	createsprite32x32(x,y,REDSTONEgfx,false,0);	
}
void REDSTONE_setup(){
	REDSTONEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*REDSTONE;
	dmaCopy(blocktiles,REDSTONEgfx,32*32);
}
