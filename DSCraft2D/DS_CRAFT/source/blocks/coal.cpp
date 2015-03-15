#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* COALgfx;
void COAL_render(int x,int y){
	createsprite32x32(x,y,COALgfx,false,0);	
}
void COAL_setup(){
	COALgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COAL;
	dmaCopy(blocktiles,COALgfx,32*32);
}
