#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* LAPISgfx;
void LAPIS_render(int x,int y){
	createsprite32x32(x,y,LAPISgfx,false,0);	
}
void LAPIS_setup(){
	LAPISgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*LAPIS;
	dmaCopy(blocktiles,LAPISgfx,32*32);
}
