#include "mining.h"
#include "world.h"
#include "player.h"
#include "inventory.h"
#include "top-screen.h"
#include "sprcount.h"
#include <nds.h>
#include "block.h"
#include "blockID.h"
#include "crafting.h"
int crafting_recipies[][13]={
LOG,AIR,AIR,AIR,AIR,AIR,AIR,AIR,AIR,1,1,PLANK,4,
PLACED_LOG,AIR,AIR,AIR,AIR,AIR,AIR,AIR,AIR,1,1,PLANK,4,
PLACED_LOG_W,AIR,AIR,AIR,AIR,AIR,AIR,AIR,AIR,1,1,PLANK,4,
PLACED_LOG_D,AIR,AIR,AIR,AIR,AIR,AIR,AIR,AIR,1,1,PLANK,4,
DARK_WOOD,AIR,AIR,AIR,AIR,AIR,AIR,AIR,AIR,1,1,PLANK,4,
WHITE_WOOD,AIR,AIR,AIR,AIR,AIR,AIR,AIR,AIR,1,1,PLANK,4,
PLANK,PLANK,AIR,PLANK,PLANK,AIR,AIR,AIR,AIR,2,2,CRAFT_TABLE,1}; //Crafting table! :D
#define AMOUNT_OF_RECIPIES 6
u16* gfx_of_blocks[3][3];
int blockId[3][3];
int block_crafting_Amount[3][3];
u16* dirt;
void crafting_2x2_menu(){
	char* blocktiles;
	playerActor* player_something;
	worldObject* world_something;
	bool inmenu=true;
	int chosen_block=255;
	touchPosition crafting_touch;
	lcdMainOnTop();
	int craft_x;
	int craft_y;
	subShowBlock(chosen_block);
	subLifes(-1);
	oamSet(&oamSub,0, //Then draw the sprite on the screen
                        45,
                        192-32, 
                        0, 
                        0,
                        SpriteSize_32x32, 
                        SpriteColorFormat_256Color, 
                        dirt, 
                        -1, 
                        false, 
                        false,                  
                        false, false, 
                        false   
                        );  
	swiWaitForVBlank();
	oamUpdate(&oamSub);   
	while(inmenu==true){

	subLifes(-1);
		//Generate the graphics for all the "blocks" in the crafting table...
		int i,j;
		if (keysDown() & KEY_R){
			iprintf("%d,%d\n%d,%d\n%d,%d\n%d,%d\n",blockId[0][0],blockId[0][1],blockId[1][0],blockId[1][1],block_crafting_Amount[0][0],block_crafting_Amount[0][1],block_crafting_Amount[1][0],block_crafting_Amount[1][1]);
		}
		int c,b;
		c=121;
		craft_x=0;
		int spritenum=5;
		for (i=0;i<=1;i++){
			for(j=0;j<=1;j++){
			blocktiles=(char*)&blockTiles;
			blocktiles+=(32*32)*blockId[i][j];
			if (blockId[i][j]<128) craft_x=1;
			else if (blockId[i][j]==AIR) craft_x=1;
			else if (blockId[i][j]==PLACED_LOG){
				blocktiles=(char*)&blockTiles;
				blocktiles+=(32*32)*LOG;
				craft_x=1;
			}	
			else if (blockId[i][j]==PLACED_LOG_D){
				blocktiles=(char*)&blockTiles;
				blocktiles+=(32*32)*DARK_WOOD;
				craft_x=1;
			}	
			else if (blockId[i][j]==PLACED_LOG_W){
				blocktiles=(char*)&blockTiles;
				blocktiles+=(32*32)*WHITE_WOOD;
				craft_x=1;
			}
			if (craft_x==1)dmaCopy(blocktiles,gfx_of_blocks[i][j],32*32);
			oamSet(&oamSub,spritenum,c,b,0,0,SpriteSize_32x32,SpriteColorFormat_256Color,gfx_of_blocks[i][j],-1,false,blockId[i][j]==AIR,false,false,false);
			spritenum++;
			b+=36;
			}
			b=31;
			c+=36;
		}
		oamSet(&oamSub,45, //Then draw the sprite on the screen
                        0, 
                        192-32, 
                        0, 
                        0,
                        SpriteSize_32x32, 
                        SpriteColorFormat_256Color, 
             		dirt, 
                        -1, 
                        false, 
                        false,                  
                        false, false, 
                        false   
                        );  
		scanKeys();
		touchRead(&crafting_touch);
		if (keysHeld() & KEY_START){
			inmenu=false;		
		}
		else if (keysDown() & KEY_X){
			lcdMainOnBottom();
			chosen_block=chooseBlock(world_something,player_something);
			lcdMainOnTop();
			resetSpriteCount();
			oamClear(&oamMain,0,127);
			oamClear(&oamSub,0,127);
			swiWaitForVBlank();
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);
		}
		//get x and y values of pen...
		//First x...
		craft_x=-1;
		craft_y=-1;
		if (crafting_touch.px>121 && crafting_touch.px<157) craft_x=0;
		else if (crafting_touch.px>157 && crafting_touch.px<192) craft_x=1;
		//Now y!
		if (crafting_touch.py>31 && crafting_touch.py<67) craft_y=0;
		else if (crafting_touch.py>67 && crafting_touch.py < 102) craft_y=1;
		if (keysDown() & KEY_TOUCH) iprintf("%d,%d\n",craft_x,craft_y);
		if (keysDown() & KEY_TOUCH && craft_x!=-1 && craft_y!=-1){
			if (inventoryRemove(chosen_block)){
				if( blockId[craft_x][craft_y]==chosen_block) block_crafting_Amount[craft_x][craft_y]++;
				else if (blockId[craft_x][craft_y]!=chosen_block){
					inventoryAddAmount(blockId[craft_x][craft_y],block_crafting_Amount[craft_x][craft_y]);	
					block_crafting_Amount[craft_x][craft_y]=1;
					blockId[craft_x][craft_y]=chosen_block;
				}
			}
		}
		//Now check if you have made a recipie...
		craft_x=0;
		craft_y=0;
		int recipie=1;
		if( blockId[0][0]==AIR && blockId[0][1]==AIR) craft_x=1;
		if(blockId[0][0]==AIR && blockId[1][0]==AIR) craft_y=1;
	//iprintf("Recipie %d\n",recipie);

		int checked[9]={0,0,0,0,0,0,0,0,0};
		if (recipie==1){
			int a;
			for (a=0;a<AMOUNT_OF_RECIPIES;a++){
				for(i=0;i<=9;i++)checked[i]=0;
				for(i=0;i<crafting_recipies[a][9];i++)
					for(j=0;j<crafting_recipies[a][10];j++){
						if (blockId[i+craft_x][j+craft_y]==crafting_recipies[a][i+(j*3)]) checked[i+(j*3)]=1;
					}			
				int count=0;
				for (i=0;i<=9;i++) if (checked[i]==1) count++;
				if( count==(crafting_recipies[a][9]*crafting_recipies[a][10])){
					//Recipie is true 217,51
					if (crafting_touch.px>217 && crafting_touch.px<217+32 && crafting_touch.py>51 && crafting_touch.py<51+32 && keysDown() & KEY_TOUCH){
					inventoryAddAmount(crafting_recipies[a][11],crafting_recipies[a][12]);
					iprintf("DEBUG:%d,%d,%d,%d,%d,%d,%d\n",count,(crafting_recipies[a][9]*crafting_recipies[a][10]),crafting_recipies[a][11],crafting_recipies[a][12],craft_x,craft_y,a);
					for (i=0;i<=3;i++)
						for (j=0;j<=3;j++){
							block_crafting_Amount[i][j]--;
							if (block_crafting_Amount[i][j]==0) blockId[i][j]=AIR;
						}	
					}		
				}					
			}	
		}
		if (!invHave(chosen_block)) chosen_block=AIR;
		DrawAmountNum(chosen_block);
		subShowBlock(chosen_block);
		swiWaitForVBlank();
		oamUpdate(&oamSub);
	}
	lcdMainOnBottom();
	oamClear(&oamSub,0,127);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
	while(keysHeld()) scanKeys();
}
void crafting_init(){
	int i;
	for (i=0;i<=3;i++)
		for (int j=0;j<=3;j++){
 	gfx_of_blocks[i][j] = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); 
	}
	dirt= oamAllocateGfx(&oamSub , SpriteSize_32x32, SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DIRT;
	dmaCopy(blocktiles,dirt,32*32);
	for (i=0;i<=3;i++){
		for (int j=0;j<=3;j++){
			blockId[i][j]=AIR;
			block_crafting_Amount[i][j]=1;

		}
	}
}
