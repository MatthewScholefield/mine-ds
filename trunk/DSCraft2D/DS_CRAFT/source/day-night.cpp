#include <nds.h>
#include <stdio.h>
#include <time.h>
#include "day-night.h"
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
//#include "mainscreen.h" //image file (Just a test background, later the tiled BG)

int h = 0;
int k = 0;

u8 theTile[64] = 
{
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1
};
int framecounter;
int r;
int g;
int b;
bool up;
timeStruct timeinworld2;
int timeinworld;
/*void mainBGUpdate(){

	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)

    int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    dmaCopy(mainscreenBitmap, bgGetGfxPtr(bg), 256*256);
    dmaCopy(mainscreenPal, BG_PALETTE, 256*2);
}*/
void mainBGSetup(){

	int i;
	u8* tileMemory = (u8*)BG_TILE_RAM(1);
	u16* mapMemory = (u16*)BG_MAP_RAM(0);
	//tell the DS where we are putting everything and set 256 color mode and that we are using a 32 by 32 tile map.
	REG_BG0CNT = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
	dmaCopy(theTile, tileMemory, 32);
	for (i=0;i<=64;i++){
		tileMemory[i]=theTile[i];
	}
	r=112; //Changing this means that I have to recode the mainBGUpdate (dont do it again PLEASE)
	g=219;
	b=255;
	BG_PALETTE[0]=RGB15(112,219,255);
	framecounter=0;
	//create a map in map memory
	for(i = 0; i < 32 * 32; i++)
		mapMemory[i] = 0;
	timeinworld2.ticks=251;
	timeinworld=251;
}
void mainBGUpdate(){
        framecounter++;
        if (framecounter%120==0){
                timeinworld++;
                k--; 
			if (k <= -20)
		    swiWaitForVBlank();
		    swiWaitForVBlank();
			h--;
			if (h >= -10)
		    setBrightness(1,h+2);         
        }
        if (timeinworld>50 && timeinworld<100)
        {
                g--;
                BG_PALETTE[0]=RGB15(r,g,b);
                if (g<219-25) timeinworld=101;
        }
        if (timeinworld>200 && timeinworld<250)
        {
                g++;
                BG_PALETTE[0]=RGB15(r,g,b);
                if (g>219) timeinworld=251;
        }
        if (timeinworld>350) timeinworld=0;
}


timeStruct* timeGet(){
	timeinworld2.ticks=timeinworld;
	timeStruct* returner=&timeinworld2;
	return returner;
}
void timeSet(int time){
	timeinworld=0;
	int i;	
	r=112; //Changing this means that I have to recode the mainBGUpdate (dont do it again PLEASE)
	g=219;
	b=255;
	for(i=0;i<=time*120;i++)
	{
		mainBGUpdate(); //Updates the BG time times
	}
	BG_PALETTE[0]=RGB15(r,g,b);
}
void timeSave(FILE* save_file){
	fwrite(&timeinworld, 1, sizeof(timeinworld), save_file);
	fwrite(&r, 1, sizeof(r), save_file);
	fwrite(&g, 1, sizeof(g), save_file);
	fwrite(&b, 1, sizeof(b), save_file);
	BG_PALETTE[0]=RGB15(r,g,b);
}
void timeLoad(FILE* save_file){
	fread(&timeinworld, 1, sizeof(timeinworld), save_file);
	fread(&r, 1, sizeof(r), save_file);
	fread(&g, 1, sizeof(g), save_file);
	fread(&b, 1, sizeof(b), save_file);
	BG_PALETTE[0]=RGB15(r,g,b);
}
