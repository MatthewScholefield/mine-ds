#include <nds.h>
#include "player.h"
#include "world.h"
#include "blockID.h"
#include "block.h"
#include "allblocks.h"
#include "gameshelper.h"
#include "sprcount.h"
#include "top-screen.h"
#include "done.h"
#include <stdio.h>
#include "colision.h"
#include "controls.h"
#include "inventory.h"
#include "crafting.h"
int block_action;
touchPosition mine_touch;
int non_placeable[]={STICK};//List of items you can not place...
#define NON_PLACEABLE_NUM 1
int mine_frame;
int mine_timeout;
u16* donegfx;
int olax, olay;
int spritecol2(int fx,int fy,int sx,int sy,int fSizex,int fSizey,int sSizex,int sSizey){
	if ((fx + fSizex > sx )&& (fx < sx+sSizex) && (sy + sSizey > fy) && (sy < fy+fSizey)) 
		return 1;
	else 
		return 0;
	return 0;
}
void drawBlock(int block,int x,int y){
	//When implementing a new block that you can pick up.. you MUST update this function as well...
	switch(block)
	{
		case STONE: STONE_render(x,y); break;
		case GRASS: GRASS_render(x,y); break;	
		case SNOW_GRASS: SNOW_GRASS_render(x,y); break;	
		case DIRT: DIRT_render(x,y); break;
		case SAND: SAND_render_nofall(x,y);	 break; //Sand needs extra parameters because it needs to "fall"
		case WHITE_WOOD: WHITE_WOOD_render(x,y); break;
		case DARK_WOOD: DARK_WOOD_render(x,y); break;
		case PLACED_LOG_W: WHITE_WOOD_render(x,y); break;
		case PLACED_LOG_D: DARK_WOOD_render(x,y); break;
		case LOG: LOG_render(x,y); break;
		case PLACED_LOG: LOG_render(x,y); break;
		case LEAVES: LEAVES_render(x,y); break;
		case PLANKS: PLANKS_render(x,y); break;
		case TORCH: TORCH_render(x,y); break;
		case COBBLE: COBBLE_render(x,y); break;
		case BEDROCK: BEDROCK_render(x,y); break;
		case DOOR_OPEN_TOP: DOOR_OPEN_render(x,y); break;
		case DOOR_OPEN_BOTTOM: DOOR_OPEN_render(x,y-32); break;
		case DOOR_CLOSED_TOP: DOOR_CLOSED_render(x,y); break;
		case DOOR_CLOSED_BOTTOM: DOOR_CLOSED_render(x,y-32); break;
		case GRAVEL: GRAVEL_render_nofall(x,y); break;
		case SANDSTONE: SANDSTONE_render(x,y); break;
		case COAL_ORE: COAL_ORE_render(x,y); break;
		case IRON_ORE: IRON_ORE_render(x,y); break;
		case GOLD_ORE: GOLD_ORE_render(x,y); break;
		case DIAMOND_ORE: DIAMOND_ORE_render(x,y); break;
		case REDSTONE_ORE: REDSTONE_ORE_render(x,y); break;
		case LAPIS_ORE: LAPIS_ORE_render(x,y); break;
		case GLASS: GLASS_render(x,y); break;
		case CACTUS: CACTUS_render(x,y); break;
	    //Here Lapis-BLOCK
		case WOOL_WHITE: WOOL_WHITE_render(x,y); break;
		case PLACED_LEAF: LEAVES_render(x,y); break;
		case FLOWER_RED: FLOWER_RED_render(x,y); break;
		case FLOWER_YELLOW: FLOWER_YELLOW_render(x,y); break;
		case CRAFT_TABLE: CRAFT_TABLE_render(x,y); break;
		case STICK: STICK_render(x,y); break;
	}
}
void doneSetup(){
	donegfx=oamAllocateGfx(&oamMain,SpriteSize_64x32,SpriteColorFormat_256Color);
	dmaCopy(doneTiles,donegfx,64*32);
	int i;
	
}
int chooseBlock(worldObject* world,playerActor* MainPlayer){
	int fertig=false;
	resetSpriteCount();
	int i=AIR;
	oamClear(&oamMain,0,127);
	//DIRT_render(0,0); //DEBUG STUFF...........
	int x=0;
	int y=0;
	int blocks[4][8];
	int a=0;
	int b;
	//Clear the blocks[4][8] array...
	for (b=0;b<=3;b++)
		for(a=0;a<=7;a++)
			blocks[b][a]=AIR;
	bool done=false;
	b=0;
	//Display all the collected blocks...
	while(y<4 && b<512){
		if (invHave(b)){
			//If you have the block "B"
			//iprintf("%d,%d,%d\n",b,x,y);
			drawBlock(b,x*32,y*32);
			blocks[y][x]=b;
			x++;
		}
		if (x==7){
			y++;
			x=0;		
		}
		b++;
	}
	oamSet(&oamMain,nextSprite(),256/2-32,192-32,0,2,SpriteSize_64x32,SpriteColorFormat_256Color,donegfx,-1,false,false,false,false,false); 
	oamUpdate(&oamMain);
	a=0;
	while(fertig==false){
		//38,32 //A reminder to myself	
		scanKeys();
		touchRead(&mine_touch);
		if (keysDown() & KEY_TOUCH){
			bool touched=false;
			//Check whether a block was touched...
			for (y=0;y<=3;y++){
				for(x=0;x<=7;x++){
					if (spritecol2(mine_touch.px,mine_touch.py,x*32,y*32,1,1,32,32)){
						i=blocks[y][x];
						//iprintf("Collision! %d %d %d\n",i,y,x);
						touched=true;				
					}
				}			
			}
			if (spritecol2(mine_touch.px,mine_touch.py,256/2-32,192-32,1,1,64,32)) fertig=true;
			else if (touched==false){
				i=AIR;
			}
			subShowBlock(i);
			DrawAmountNum(i);
		}

			swiWaitForVBlank();
			oamUpdate(&oamSub);
	
	}
	//iprintf("Current i %d", i);
	while(keysHeld() & KEY_TOUCH) scanKeys();
	return i;
}
void miningUpdate(worldObject* CurrentWorld,playerActor* MainPlayer){
		if (keysDown() & KEY_R || keysDown() & KEY_Y){
			CurrentWorld->ChoosedBlock = AIR;
		}         
		if (keysDown() & KEY_L || keysDown() & KEY_X){
           		CurrentWorld->ChoosedBlock=chooseBlock(CurrentWorld,MainPlayer);
		}
		if (keysDown() & KEY_TOUCH){
			mine_touch=controlsTouch();
			int x=mine_touch.px;
			int y=mine_touch.py;
			x+= CurrentWorld->CamX;
			y+=CurrentWorld->CamY;
			int i,j;
			int lax,lay;
			MainPlayer->facing_left=x<MainPlayer->x;
			for(i=MainPlayer->blockx-4;i<=MainPlayer->blockx+4;i++)
				for(j=MainPlayer->blocky-4;j<=MainPlayer->blocky+4;j++)
					if (spritecol2(x,y,i*32,j*32,1,1,32,32)){
						lax=i;
						lay=j;
						j=WORLD_HEIGHT+2;
						i=WORLD_WIDTH+2;
					}
			if (CurrentWorld->blocks[lax][lay]!=AIR) block_action=0;
			else block_action=1;
		}
		if (keysHeld() & KEY_TOUCH){
			mine_frame++;
			mine_touch=controlsTouch();
			int x=mine_touch.px;
			int y=mine_touch.py;
			x+= CurrentWorld->CamX;
			y+=CurrentWorld->CamY;
			int i,j;
			int lax,lay; //The block position of the pen...
			MainPlayer->facing_left=x<MainPlayer->x;
			for(i=MainPlayer->blockx-4;i<=MainPlayer->blockx+4;i++)
				for(j=MainPlayer->blocky-4;j<=MainPlayer->blocky+4;j++)
					if (spritecol2(x,y,i*32,j*32,1,1,32,32)){
						lax=i;
						lay=j;
						j=WORLD_HEIGHT+2;
						i=WORLD_WIDTH+2;
					}
			if (olay!=lay || olax!=lax) mine_frame=0; //If the last position is not the same as the current position, then reset the mining time...
			olay=lay; //Create a variable of the last position...
			olax=lax;
			if (CurrentWorld->blocks[lax][lay]!=AIR) PlayerPunch(MainPlayer);
				if (block_action==0 && !(keysHeld() & KEY_B || keysHeld() & KEY_DOWN )){ //REmoving blocks
					switch(CurrentWorld->blocks[lax][lay])
					{
						case GRASS:  GRASS_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case DIRT:  DIRT_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case SNOW_GRASS:  SNOW_GRASS_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case SAND:  SAND_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case SANDSTONE:  SANDSTONE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case STONE:  STONE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case CACTUS:  CACTUS_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case COAL_ORE:  COAL_ORE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case COBBLE: COBBLE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case DIAMOND_ORE: DIAMOND_ORE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case GLASS: GLASS_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case GOLD_ORE: GOLD_ORE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case IRON_ORE: IRON_ORE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case REDSTONE_ORE: REDSTONE_ORE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case LAPIS_ORE: LAPIS_ORE_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case GRAVEL: GRAVEL_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case LEAVES: LEAVES_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLACED_LEAF: LEAVES_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case LOG: LOG_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case WHITE_WOOD: WHITE_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLACED_LOG: LOG_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLACED_LOG_W: WHITE_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLACED_LOG_D: DARK_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case DARK_WOOD: DARK_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLANKS: PLANKS_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case TORCH: TORCH_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case FLOWER_RED: FLOWER_RED_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case FLOWER_YELLOW: FLOWER_YELLOW_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case CRAFT_TABLE: CRAFT_TABLE_mine(CurrentWorld,&mine_frame,lax,lay); break;

					}
				}
				else if (block_action==1 && (lax!=MainPlayer->blockx || lay!=MainPlayer->blocky) && !(keysHeld() & KEY_B || keysHeld() & KEY_DOWN) && mine_timeout==0){ //Adding Blocks...
						if (CurrentWorld->blocks[lax][lay]==AIR ){ // You can only place blocks on air...
							//First check that you are allowed to place this plock...
							//Basically DONT PLACE ITEMS
							bool allow=true;
							int place_scan=0;
							for (place_scan=0;place_scan<NON_PLACEABLE_NUM;place_scan++){
								if (CurrentWorld->ChoosedBlock==non_placeable[place_scan]) allow=false;
							}
							if (allow)
								if (inventoryRemove(CurrentWorld->ChoosedBlock)) CurrentWorld->blocks[lax][lay]=CurrentWorld->ChoosedBlock; 
						mine_timeout=31;
						}
				}
				else if (block_action==0){
					if( CurrentWorld->blocks[lax][lay]==CRAFT_TABLE) crafting_3x3_menu();				
				}
				//if (CurrentWorld->blocks[lax][lay]==AIR) mine_frame=0;
			}
		else {
			mine_frame=0;		
		}
		if (mine_timeout>0) mine_timeout--;
}
