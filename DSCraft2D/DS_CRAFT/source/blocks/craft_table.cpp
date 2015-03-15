#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../sounds.h"
#include "../inventory.h"
u16* CRAFT_TABLEgfx;
int CRAFT_TABLEls;
void CRAFT_TABLE_render(int x,int y){
	createsprite32x32(x,y,CRAFT_TABLEgfx,false,0);	
}
void CRAFT_TABLE_setup(){
	CRAFT_TABLEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*CRAFT_TABLE;
	dmaCopy(blocktiles,CRAFT_TABLEgfx,32*32);
}
void CRAFT_TABLE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (CRAFT_TABLEls==0){
				playSound(WOOD_A);		
				CRAFT_TABLEls++;
			}
			else if (CRAFT_TABLEls==1){
				playSound(WOOD_B);
				CRAFT_TABLEls++;
			}	
			else if (CRAFT_TABLEls==2){
				playSound(WOOD_C);
				CRAFT_TABLEls++;
			}			
			else if (CRAFT_TABLEls==3){
				playSound(WOOD_D);
				CRAFT_TABLEls=0;
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
	}//Should there be colision for the crafting table?
}
void CRAFT_TABLE_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>225 && world->ChoosedBlock!= WOOD_AXE && world->ChoosedBlock!=COBBLE_AXE && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_D);
		*mine_time=0;
	}
	else if(*mine_time>225 && world->ChoosedBlock!= WOOD_AXE && world->ChoosedBlock!=COBBLE_AXE && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_D);
		*mine_time=0;
		setData(world->ChoosedBlock,2,true); //Add 1 "use" to the asdf
	}
	else if (*mine_time>114 && world->ChoosedBlock==WOOD_AXE){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_D);
		setData(WOOD_PICK,1,true); //Add 1 "use" to the pickaxe...
		*mine_time=0;
	}
	else if (*mine_time>57 && world->ChoosedBlock==COBBLE_AXE){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_D);
		setData(WOOD_PICK,1,true); //Add 1 "use" to the pickaxe...
		*mine_time=0;
	}
}
