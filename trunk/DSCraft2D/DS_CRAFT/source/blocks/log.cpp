#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../sounds.h"
u16* LOGgfx;

int woodls;
void LOG_render(int x,int y){
	createsprite32x32(x,y,LOGgfx,false,0);	
}
void LOG_setup(){
	LOGgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*LOG;
	dmaCopy(blocktiles,LOGgfx,32*32);
}
void LOG_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (woodls==0){
				playSound(WOOD_A);		
				woodls++;
			}
			else if (woodls==1){
				playSound(WOOD_B);
				woodls++;
			}	
			else if (woodls==2){
				playSound(WOOD_C);
				woodls++;
			}			
			else if (woodls==3){
				playSound(WOOD_D);
				woodls=0;
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
