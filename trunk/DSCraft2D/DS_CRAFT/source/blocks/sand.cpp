#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../sounds.h"
#include "../inventory.h"
u16* SANDgfx;
int sandls;
void SAND_render(int x,int y){
	createsprite32x32(x,y,SANDgfx,false,0);	
}
void SAND_setup(){
	SANDgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*SAND;
	dmaCopy(blocktiles,SANDgfx,32*32);
}
void SAND_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (sandls==0){
				playSound(SAND_A);		
				sandls++;
			}
			else if (sandls==1){
				playSound(SAND_B);
				sandls++;
			}	
			else if (sandls==2){
				playSound(SAND_C);
				sandls++;
			}			
			else if (sandls==3){
				playSound(SAND_D);
				sandls=0;
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
void SAND_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>45){
		world->blocks[x][y]=AIR;
		inventoryAdd(SAND);
		*mine_time=0;
	}
}
