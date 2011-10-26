#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../sounds.h"
u16* LAPIS_OREgfx;

int lapis_orelx,lapis_orely,lapis_orels;
void LAPIS_ORE_render(int x,int y){
	createsprite32x32(x,y,LAPIS_OREgfx,false,0);	
}
void LAPIS_ORE_setup(){
	LAPIS_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*LAPIS_ORE;
	dmaCopy(blocktiles,LAPIS_OREgfx,32*32);
}
void LAPIS_ORE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==lapis_orelx && by==lapis_orely)){
			lapis_orelx=bx,lapis_orely=by;
			if (lapis_orels==0){
				playSound(STONE_A);		
				lapis_orels++;
			}
			else if (lapis_orels==1){
				playSound(STONE_B);
				lapis_orels++;
			}	
			else if (lapis_orels==2){
				playSound(STONE_C);
				lapis_orels++;
			}			
			else if (lapis_orels==3){
				playSound(STONE_D);
				lapis_orels=0;
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
		player->y+=2;	
	}
}
