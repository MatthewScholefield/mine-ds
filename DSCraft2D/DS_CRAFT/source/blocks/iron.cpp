#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* IRONgfx;
void IRON_render(int x,int y){
	createsprite32x32(x,y,IRONgfx,false,0);	
}
void IRON_setup(){
	IRONgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*IRON;
	dmaCopy(blocktiles,IRONgfx,32*32);
}
