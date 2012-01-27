#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* COAL_OREgfx;
int coalls;
void COAL_ORE_render(int x,int y){
	createsprite32x32(x,y,COAL_OREgfx,false,0);	
}
void COAL_ORE_setup(){
	COAL_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COAL_ORE;
	dmaCopy(blocktiles,COAL_OREgfx,32*32);
}
void COAL_ORE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (coalls==0){
				playSound(STONE_A);		
				coalls++;
			}
			else if (coalls==1){
				playSound(STONE_B);
				coalls++;
			}	
			else if (coalls==2){
				playSound(STONE_C);
				coalls++;
			}			
			else if (coalls==3){
				playSound(STONE_D);
				coalls=0;
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
void COAL_ORE_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>900 && world->ChoosedBlock!= WOOD_PICK && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		*mine_time=0;
	}
	else if(*mine_time>900 && world->ChoosedBlock!= WOOD_PICK && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		*mine_time=0;
		setData(world->ChoosedBlock,2,true); //Add 1 "use" to the pickaxe...
	}
	else if (*mine_time>135 && world->ChoosedBlock==WOOD_PICK){
		world->blocks[x][y]=AIR;
		inventoryAdd(COAL);
		setData(WOOD_PICK,1,true); //Add 1 "use" to the pickaxe...
		*mine_time=0;
	}
}
