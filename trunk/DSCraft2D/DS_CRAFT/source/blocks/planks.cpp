#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../sounds.h"
#include "../inventory.h"
u16* PLANKSgfx;
int planksls;
void PLANKS_render(int x,int y){
	createsprite32x32(x,y,PLANKSgfx,false,0);	
}
void PLANKS_setup(){
	PLANKSgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*PLANKS;
	dmaCopy(blocktiles,PLANKSgfx,32*32);
}
void PLANKS_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (planksls==0){
				playSound(WOOD_A);		
				planksls++;
			}
			else if (planksls==1){
				playSound(WOOD_B);
				planksls++;
			}	
			else if (planksls==2){
				playSound(WOOD_C);
				planksls++;
			}			
			else if (planksls==3){
				playSound(WOOD_D);
				planksls=0;
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
void PLANKS_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>180){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLANKS);
		*mine_time=0;
	}
}
