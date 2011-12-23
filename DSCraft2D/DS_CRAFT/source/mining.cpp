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
int block_action;
touchPosition mine_touch;
int mine_frame;
u16* donegfx;
int olax, olay;
int spritecol2(int fx,int fy,int sx,int sy,int fSizex,int fSizey,int sSizex,int sSizey){
	if ((fx + fSizex > sx )&& (fx < sx+sSizex) && (sy + sSizey > fy) && (sy < fy+fSizey)) 
		return 1;
	else 
		return 0;
	return 0;
}
void doneSetup(){
	donegfx=oamAllocateGfx(&oamMain,SpriteSize_64x32,SpriteColorFormat_256Color);
	dmaCopy(doneTiles,donegfx,64*32);
}
int chooseBlock(worldObject* world,playerActor* MainPlayer){
	int fertig=false;
	resetSpriteCount();
	int i=AIR;
	oamClear(&oamMain,0,127);
	DIRT_render(0,0);
	STONE_render(32,0);
	COBBLE_render(64,0);
	PLANKS_render(96,0);
	LOG_render(128,0);
	TORCH_render(160,0);
	SAND_render_nofall(192,0);
	SANDSTONE_render(224,0);
	GRAVEL_render(0,32);
	DARK_WOOD_render(32,32);
	WHITE_WOOD_render(64,32);
	LEAVES_render(96,32);
	COAL_ORE_render(128,32);
	IRON_ORE_render(160,32);
	GOLD_ORE_render(192,32);
	REDSTONE_ORE_render(224,32);
	DIAMOND_ORE_render(0,64);
	LAPIS_ORE_render(32,64);
	GLASS_render(64,64);
	CACTUS_render(96,64);
	//Here Lapis-BLOCK
	WOOL_WHITE_render(128,64);
	SNOW_GRASS_render(160,64);
	FLOWER_RED_render(192,64);
	oamSet(&oamMain,nextSprite(),256/2-32,192-32,0,2,SpriteSize_64x32,SpriteColorFormat_256Color,donegfx,-1,false,false,false,false,false); 
	oamUpdate(&oamMain);
	while(fertig==false){
		//38,32 //A reminder to myself	
		scanKeys();
		touchRead(&mine_touch);
		if (keysDown() & KEY_TOUCH){
			if (spritecol2(mine_touch.px,mine_touch.py,0,0,1,1,32,32)){
				i=DIRT;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,32,0,1,1,32,32)){
				i=STONE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,64,0,1,1,32,32)){
				i=COBBLE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,96,0,1,1,32,32)){
				i=WOOD;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,128,0,1,1,32,32)){
				i=PLACED_LOG;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,160,0,1,1,32,32)){
				i=TORCH;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,192,0,1,1,32,32)){
				i=SAND;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,224,0,1,1,32,32)){
				i=SANDSTONE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,32,32,1,1,32,32)){
				i=PLACED_LOG_D;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,64,32,1,1,32,32)){
				i=PLACED_LOG_W;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,96,32,1,1,32,32)){
				i=PLACED_LEAF;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,0,32,1,1,32,32)){
				i=GRAVEL;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,128,32,1,1,32,32)){
				i=COAL_ORE;
			}			
			else if (spritecol2(mine_touch.px,mine_touch.py,160,32,1,1,32,32)){
				i=IRON_ORE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,192,32,1,1,32,32)){
				i=GOLD_ORE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,224,32,1,1,32,32)){
				i=REDSTONE_ORE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,0,64,1,1,32,32)){
				i=DIAMOND_ORE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,32,64,1,1,32,32)){
				i=LAPIS_ORE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,64,64,1,1,32,32)){
				i=GLASS;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,96,64,1,1,32,32)){
				i=CACTUS;
			}
			//Here Lapis-Block
			else if (spritecol2(mine_touch.px,mine_touch.py,128,64,1,1,32,32)){
				i=WOOL_WHITE;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,160,64,1,1,32,32)){
				i=SNOW_GRASS;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,192,64,1,1,32,32)){
				i=FLOWER_RED;
			}
			else if (spritecol2(mine_touch.px,mine_touch.py,256/2-32,192-32,1,1,64,32)) fertig=true;
			else {
				i=AIR;
			}
			subShowBlock(i);
			DrawAmountNum(i);
		}

			swiWaitForVBlank();
			oamUpdate(&oamSub);
	
	}
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
				if (block_action==0){ //REmoving blocks
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
						case LOG: LOG_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case WHITE_WOOD: WHITE_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLACED_LOG: LOG_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLACED_LOG_W: WHITE_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLACED_LOG_D: DARK_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case DARK_WOOD: DARK_WOOD_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case PLANKS: PLANKS_mine(CurrentWorld,&mine_frame,lax,lay); break;
						case TORCH: TORCH_mine(CurrentWorld,&mine_frame,lax,lay); break;

					}
				}
				else if (block_action==1 && (lax!=MainPlayer->blockx || lay!=MainPlayer->blocky) ){ //Adding Blocks...
						if (CurrentWorld->blocks[lax][lay]==AIR){ // You can only place blocks on air...
							if (inventoryRemove(CurrentWorld->ChoosedBlock)) CurrentWorld->blocks[lax][lay]=CurrentWorld->ChoosedBlock; 
						}
				}
				if (CurrentWorld->blocks[lax][lay]==AIR) mine_frame=0;
			}
		else {
			mine_frame=0;		
		}
}
