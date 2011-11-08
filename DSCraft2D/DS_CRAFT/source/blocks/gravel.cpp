#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
u16* GRAVELgfx;
int gravells=0;
void GRAVEL_render(int x,int y){
	createsprite32x32(x,y,GRAVELgfx,false,0);	
}
void GRAVEL_setup(){
	GRAVELgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*GRAVEL;
	dmaCopy(blocktiles,GRAVELgfx,32*32);
}
void GRAVEL_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (gravells==0){
				playSound(GRAVEL_A);		
				gravells++;
			}
			else if (gravells==1){
				playSound(GRAVEL_B);
				gravells++;
			}	
			else if (gravells==2){
				playSound(GRAVEL_C);
				gravells++;
			}			
			else if (gravells==3){
				playSound(GRAVEL_D);
				gravells=0;
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
