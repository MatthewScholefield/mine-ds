#include <nds.h>
#include <stdio.h>
 

//-------Sprites---------//
#include "Mariotest.h"
#include "Mariotest2.h"
//-----------------------//


int x1,y1=176;

void grav(){
  y1+=1;
  }
void col(){
  if (y1 >=176) 
  y1-=1;
  }

int main(){

videoSetMode(MODE_5_2D);//
vramSetBankA(VRAM_A_MAIN_SPRITE);
oamInit(&oamMain,SpriteMapping_1D_32,true);
vramSetBankF(VRAM_F_LCD);

int i;
for (i=0;i<=MariotestPalLen;i++)
VRAM_F_EXT_SPR_PALETTE[0][i] = MariotestPal[i];
for (i=0;i<=Mariotest2PalLen;i++)
VRAM_F_EXT_SPR_PALETTE[1][i] = Mariotest2Pal[i];
vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
u16* gfx1 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
dmaCopy(MariotestTiles, gfx1, MariotestTilesLen);
u16* gfx2 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
dmaCopy(Mariotest2Tiles, gfx2, Mariotest2TilesLen);

oamSet(&oamMain, 
0,x1,y1,0,0,SpriteSize_16x16,SpriteColorFormat_256Color,gfx1,-1,false,false,false,false,false);

while(1){

scanKeys();
        col();
        grav();

		int held = keysHeld();
 
		if( held & KEY_A){
	     oamSet(&oamMain, 
         0,x1,y1,0,1,SpriteSize_16x16,SpriteColorFormat_256Color,gfx2,-1,false,false,false,false,false);
		 y1-=2;
		 }
		else{
         oamSet(&oamMain, 
         0,x1,y1,0,0,SpriteSize_16x16,SpriteColorFormat_256Color,gfx1,-1,false,false,false,false,false);
		 //while(y1<=176){
		 //y1-=1;
		 //  }
		 }
		 
		if( held & KEY_LEFT)
		  x1-=1;
		if( held & KEY_RIGHT)
		  x1+=1;
 
 
  swiWaitForVBlank();
  oamUpdate(&oamMain);
   }
   return(0);
}