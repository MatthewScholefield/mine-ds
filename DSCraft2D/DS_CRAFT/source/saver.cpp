#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include "player.h"
#include "world.h"
#include "controls.h"
#include "world_img.h"
#include "numbers.h"
#include "load.h"
#include "blockID.h"
#include "save.h"
#include "day-night.h"
#include "mob.h"
#include "top-screen.h"
#include "worldgen.h"
bool fatinited;
touchPosition touch2;
u16* world_gfx;
u16* load_gfx;
u16* save_gfx;
u16* numbers[10];
char worldName [30] = "fat:/world_01.mcl\0\0";

//2= world
//3= numbers
//4= load
//5= save
void renderSaveScreen(){
	blankTopScreen();//Clear the sub screen
	int spritecounter=0;
	int i;
	for (i=0;i<=4;i++){
		oamSet(&oamSub,spritecounter,0,i*36,0,2,SpriteSize_64x32,SpriteColorFormat_256Color,world_gfx,-1,false,false,false, false,false); 
		spritecounter++;
		oamSet(&oamSub,spritecounter,64,i*36,0,3,SpriteSize_16x16,SpriteColorFormat_256Color,numbers[i],-1,false,false,false, false,false); 
		spritecounter++;
		oamSet(&oamSub,spritecounter,256-(66*2),i*36,0,5,SpriteSize_64x32,SpriteColorFormat_256Color,save_gfx,-1,false,false,false, false,false); 
		spritecounter++;
		oamSet(&oamSub,spritecounter,256-66,i*36,0,4,SpriteSize_64x32,SpriteColorFormat_256Color,load_gfx,-1,false,false,false, false,false); 
		spritecounter++;
	}
 	swiWaitForVBlank();
	oamUpdate(&oamSub);
}
void saveMenu(worldObject* world,playerActor* player){
        lcdSwap();
	renderSaveScreen();
        worldObject STUF;
	timeStruct STUFFF; //This is silly :P
        bool fertig=false;
        while(!fertig){
                scanKeys();
                if (keysDown() & KEY_TOUCH){    
                        touchRead(&touch2);
           		// Get the row that the pen is on.
			int i;
			int row=0;
			for (i=0;i<=4;i++){
				if (touch2.py>i*36 && touch2.py<i*36+36) row=i;
			}
			//Row now holds the world number
			if (touch2.px<256-66 && touch2.px>256-(66*2)){
				//Save the world		
				worldName[12]=row+'0';
				FILE* save_file = fopen(worldName, "w");         
				fwrite(world, 1, sizeof(STUF), save_file);
				mobsSave(save_file);
				timeStruct* time_save;
				time_save=timeGet();
				fwrite(time_save, 1, sizeof(STUFFF), save_file);
				fclose(save_file);	
			}
			else if (touch2.px>256-66){
				//Load the world		
				worldName[12]=row+'0';
				FILE* save_file = fopen(worldName, "r");         
				fread(world, 1, sizeof(STUF), save_file);
				mobsLoad(save_file);
				timeStruct* time_save;
				fread(time_save, 1, sizeof(STUFFF), save_file);
				timeSet(time_save->ticks);
				fclose(save_file);	
			}
                        fertig=true;
                }
        }
        lcdMainOnBottom();
	unBlankTopScreen();
	oamClear(&oamSub,0,128);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
	int count;
	for (count=0;count<=60;count++) swiWaitForVBlank();
}
void saveUpdate(worldObject* world,playerActor* player){

        worldObject STUF;
        if (controlsKeysH() & KEY_START){
		saveMenu(world,player);
        }
	else if (controlsKeysH() & KEY_SELECT){
		generateWorld(world);
		int i;
		for (i=0;i<=WORLD_HEIGHT;i++)
                if (world->blocks[WORLD_WIDTH/2][i]!=AIR)
                {
                        player->y=i*32-64;
                        player->x=WORLD_WIDTHpx/2;
                        i=WORLD_HEIGHT+1;
                }
		mobSetup();
		timeStruct time;
		time.ticks=0;
		timeSet(time.ticks);
	}
}
void saveInit(){
        fatinited=fatInitDefault();
	fatinited=true;
        world_gfx = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_256Color);
        load_gfx = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_256Color);
        save_gfx = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_256Color);
	numbers[0] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	numbers[1] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	numbers[2] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	numbers[3] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	numbers[4] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
        char* numbersgfx;//Copy the graphics into memory
        numbersgfx=(char*)numbersTiles;
        dmaCopy(numbersgfx,numbers[0],16*16);
	numbersgfx+=16*16;
        dmaCopy(numbersgfx,numbers[1],16*16);
	numbersgfx+=16*16;
        dmaCopy(numbersgfx,numbers[2],16*16);
	numbersgfx+=16*16;
        dmaCopy(numbersgfx,numbers[3],16*16);
	numbersgfx+=16*16;
        dmaCopy(numbersgfx,numbers[4],16*16);
        dmaCopy(world_imgTiles,world_gfx,64*32);
        dmaCopy(loadTiles,load_gfx,64*32);
        dmaCopy(saveTiles,save_gfx,64*32);
}

