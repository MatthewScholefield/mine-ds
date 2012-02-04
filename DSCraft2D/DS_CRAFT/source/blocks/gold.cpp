#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* GOLDgfx;
void GOLD_render(int x,int y){
	createsprite32x32(x,y,GOLDgfx,false,0);	
}
void GOLD_setup(){
	GOLDgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*GOLD;
	dmaCopy(blocktiles,GOLDgfx,32*32);
}
