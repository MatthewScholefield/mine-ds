#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* WOOD_SHOVELgfx;
void WOOD_SHOVEL_render(int x,int y){
	createsprite32x32(x,y,WOOD_SHOVELgfx,false,0);	
}
void WOOD_SHOVEL_setup(){
	WOOD_SHOVELgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*WOOD_SHOVEL;
	dmaCopy(blocktiles,WOOD_SHOVELgfx,32*32);
}
