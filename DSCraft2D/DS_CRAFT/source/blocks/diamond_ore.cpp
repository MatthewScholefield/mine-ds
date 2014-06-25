#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* DIAMOND_OREgfx;
int diamond_orels;
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
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
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
		player->y=by*32+33;
	}
}
void DIAMOND_ORE_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>900 && world->ChoosedBlock!=IRON_PICKAXE && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		*mine_time=0;
	}
	else if(*mine_time>900 && world->ChoosedBlock!=IRON_PICKAXE && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		*mine_time=0;
		setData(world->ChoosedBlock,2,true); 
	}
	else if (*mine_time>45 && world->ChoosedBlock== IRON_PICKAXE){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIAMOND);
		setData(IRON_PICKAXE,1,true); //Add 1 "use" to the pickaxe...
		*mine_time=0;
	}
}