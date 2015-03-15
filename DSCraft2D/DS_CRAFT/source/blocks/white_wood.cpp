#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* WHITE_WOODgfx;
int wood_whitels;
void WHITE_WOOD_render(int x,int y){
	createsprite32x32(x,y,WHITE_WOODgfx,false,0);	
}
void WHITE_WOOD_setup(){
	WHITE_WOODgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*WHITE_WOOD;
	dmaCopy(blocktiles,WHITE_WOODgfx,32*32);
}
void WHITE_WOOD_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (wood_whitels==0){
				playSound(WOOD_A);		
				wood_whitels++;
			}
			else if (wood_whitels==1){
				playSound(WOOD_B);
				wood_whitels++;
			}	
			else if (wood_whitels==2){
				playSound(WOOD_C);
				wood_whitels++;
			}			
			else if (wood_whitels==3){
				playSound(WOOD_D);
				wood_whitels=0;
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
void WHITE_WOOD_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>180 && world->ChoosedBlock!= WOOD_AXE && world->ChoosedBlock!=COBBLE_AXE && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_W);
		*mine_time=0;
	}
	else if(*mine_time>180 && world->ChoosedBlock!= WOOD_AXE && world->ChoosedBlock!=COBBLE_AXE && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_W);
		*mine_time=0;
		setData(world->ChoosedBlock,2,true); //Add 1 "use" to the asdf
	}
	else if (*mine_time>90 && world->ChoosedBlock==WOOD_AXE){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_W);
		setData(WOOD_PICK,1,true); //Add 1 "use" to the pickaxe...
		*mine_time=0;
	}
	else if (*mine_time>45 && world->ChoosedBlock==COBBLE_AXE){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LOG_W);
		setData(WOOD_PICK,1,true); //Add 1 "use" to the pickaxe...
		*mine_time=0;
	}
}
