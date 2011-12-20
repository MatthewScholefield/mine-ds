#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* DARK_WOODgfx;

int wood_darkls;
void DARK_WOOD_render(int x,int y){
	createsprite32x32(x,y,DARK_WOODgfx,false,0);	
}
void DARK_WOOD_setup(){
	DARK_WOODgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DARK_WOOD;
	dmaCopy(blocktiles,DARK_WOODgfx,32*32);
}
void DARK_WOOD_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (wood_darkls==0){
				playSound(WOOD_A);		
				wood_darkls++;
			}
			else if (wood_darkls==1){
				playSound(WOOD_B);
				wood_darkls++;
			}	
			else if (wood_darkls==2){
				playSound(WOOD_C);
				wood_darkls++;
			}			
			else if (wood_darkls==3){
				playSound(WOOD_D);
				wood_darkls=0;
			}
		}
	}
	if (result==3){//player colides on right
		player->x-=2; //Move him back one so he is not coliding anymore :)
	}
	if (result==1){//colides on left
		player->x+=2;
	}
	if (result==4){
		//colision upwards
		player->vy=0;
		player->y=by*32+33;
	}
}
void DARK_WOOD_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>180){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_D);
		*mine_time=0;
	}
}
