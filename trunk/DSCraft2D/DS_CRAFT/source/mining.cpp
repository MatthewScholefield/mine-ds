#include <nds.h>
#include "player.h"
#include "world.h"
#include "blockID.h"
#include "block.h"
#include "allblocks.h"
#include "gameshelper.h"
#include "sprcount.h"
#include "done.h"
#include <stdio.h>
touchPosition touch;
u16* donegfx;
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
	SAND_render(192,0);
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
	oamSet(&oamMain,nextSprite(),256/2-32,192-32,0,2,SpriteSize_64x32,SpriteColorFormat_256Color,donegfx,-1,false,false,false,false,false); 
	oamUpdate(&oamMain);
	while(fertig==false){	
		scanKeys();
		touchRead(&touch);
		printf("akdf");
		if (keysHeld() & KEY_TOUCH){
			if (spritecol2(touch.px,touch.py,0,0,1,1,32,32)){
				i=DIRT;
			}
			else if (spritecol2(touch.px,touch.py,32,0,1,1,32,32)){
				i=STONE;
			}
			else if (spritecol2(touch.px,touch.py,64,0,1,1,32,32)){
				i=COBBLE;
			}
			else if (spritecol2(touch.px,touch.py,96,0,1,1,32,32)){
				i=WOOD;
			}
			else if (spritecol2(touch.px,touch.py,128,0,1,1,32,32)){
				i=PLACED_LOG;
			}
			else if (spritecol2(touch.px,touch.py,160,0,1,1,32,32)){
				i=TORCH;
			}
			else if (spritecol2(touch.px,touch.py,192,0,1,1,32,32)){
				i=SAND;
			}
			else if (spritecol2(touch.px,touch.py,224,0,1,1,32,32)){
				i=SANDSTONE;
			}
			else if (spritecol2(touch.px,touch.py,32,32,1,1,32,32)){
				i=PLACED_LOG_D;
			}
			else if (spritecol2(touch.px,touch.py,64,32,1,1,32,32)){
				i=PLACED_LOG_W;
			}
			else if (spritecol2(touch.px,touch.py,96,32,1,1,32,32)){
				i=PLACED_LEAF;
			}
			else if (spritecol2(touch.px,touch.py,0,32,1,1,32,32)){
				i=GRAVEL;
			}
			else if (spritecol2(touch.px,touch.py,128,32,1,1,32,32)){
				i=COAL_ORE;
			}			
			else if (spritecol2(touch.px,touch.py,160,32,1,1,32,32)){
				i=IRON_ORE;
			}
			else if (spritecol2(touch.px,touch.py,192,32,1,1,32,32)){
				i=GOLD_ORE;
			}
			else if (spritecol2(touch.px,touch.py,224,32,1,1,32,32)){
				i=REDSTONE_ORE;
			}
			else if (spritecol2(touch.px,touch.py,0,64,1,1,32,32)){
				i=DIAMOND_ORE;
			}
			else if (spritecol2(touch.px,touch.py,32,64,1,1,32,32)){
				i=LAPIS_ORE;
			}
			else if (spritecol2(touch.px,touch.py,64,64,1,1,32,32)){
				i=GLASS;
			}
			else if (spritecol2(touch.px,touch.py,256/2-32,192-32,1,1,64,32)) fertig=true;
			else {
				i=AIR;
			}
		}
	
	}
	while(keysHeld() & KEY_TOUCH) scanKeys();
	return i;
}
void miningUpdate(worldObject* CurrentWorld,playerActor* MainPlayer){
		scanKeys();
		if (keysDown() & KEY_R){
			CurrentWorld->ChoosedBlock = AIR;
			}
		else if (keysDown() & KEY_SELECT){
		    CurrentWorld->ChoosedBlock = AIR;
		}
		if (keysDown() & KEY_L){
           CurrentWorld->ChoosedBlock=chooseBlock(CurrentWorld,MainPlayer);
		   }
		else if (keysDown() & KEY_START){
             CurrentWorld->ChoosedBlock=chooseBlock(CurrentWorld,MainPlayer);
			 }
		if (keysHeld() & KEY_TOUCH){
			touchRead(&touch);
			int x=touch.px;
			int y=touch.py;
			x+= CurrentWorld->CamX;
			y+=CurrentWorld->CamY;
			int i,j;
			int lax,lay;
			for(i=0;i<=WORLD_WIDTH;i++)
				for(j=0;j<=WORLD_HEIGHT;j++)
					if (spritecol2(x,y,i*32,j*32,1,1,32,32)){
						lax=i;
						lay=j;
						j=WORLD_HEIGHT+2;
						i=WORLD_WIDTH+2;
					}
        		if (CurrentWorld->blocks[lax][lay]!=BEDROCK && (lax!=MainPlayer->blockx || (lay!=MainPlayer->blocky && lay!=MainPlayer->blocky-1))) CurrentWorld->blocks[lax][lay]=CurrentWorld->ChoosedBlock; 
			//the lax!=MainPlayer->**** stuff makes your your not placing a block inside the player
		}
}
