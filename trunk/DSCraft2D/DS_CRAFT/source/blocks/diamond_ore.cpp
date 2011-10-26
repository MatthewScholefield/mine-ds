#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
u16* DIAMOND_OREgfx;
int diamond_orelx,diamond_orely,diamond_orels;
void DIAMOND_ORE_render(int x,int y){
	createsprite32x32(x,y,DIAMOND_OREgfx,false,0);	
}
void DIAMOND_ORE_setup(){
	DIAMOND_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DIAMOND_ORE;
	dmaCopy(blocktiles,DIAMOND_OREgfx,32*32);
}
void DIAMOND_ORE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==diamond_orelx && by==diamond_orely)){
			diamond_orelx=bx,diamond_orely=by;
			if (diamond_orels==0){
				playSound(STONE_A);		
				diamond_orels++;
			}
			else if (diamond_orels==1){
				playSound(STONE_B);
				diamond_orels++;
			}	
			else if (diamond_orels==2){
				playSound(STONE_C);
				diamond_orels++;
			}			
			else if (diamond_orels==3){
				playSound(STONE_D);
				diamond_orels=0;
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
