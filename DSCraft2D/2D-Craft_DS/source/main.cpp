#include <nds.h>

#include "TitleBottom.h"
#include "TitleTop.h"
#include "SingleplLeft.h"
#include "SingleplMid.h"
#include "SingleplRight.h"
#include "PlayerR.h"
#include "PlayerL.h"

int count;
int e = 0;
int Spr1X,Spr2X,Spr1Y,Spr2Y,Spr3X,Spr3Y,PlX,PlY;

void timer()
{
		scanKeys();
		int held = keysHeld();
 
		if( held & KEY_LEFT)
		PlX = PlX -1;
 
		if( held & KEY_RIGHT)
		PlX = PlX +1;
		
 		if( held & KEY_UP)
		PlY = PlY -1;
		
		if( held & KEY_DOWN)
		PlY = PlY +1;
		
		if( held & KEY_TOUCH)
		PlY = PlY -1;

}


int main(void)
{

   	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

        int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

        dmaCopy(TitleTopBitmap, bgGetGfxPtr(bg), 256*256);
        dmaCopy(TitleTopPal, BG_PALETTE, 256*2);
  
  
    videoSetModeSub(MODE_5_2D);
   	vramSetBankC(VRAM_C_SUB_BG_0x06200000);

        int bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

        dmaCopy(TitleBottomBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(TitleBottomPal, BG_PALETTE_SUB, 256*2);


  videoSetMode(MODE_5_2D);
  vramSetBankF(VRAM_F_MAIN_SPRITE);
  oamInit(&oamMain,SpriteMapping_1D_32,true);
  vramSetBankF(VRAM_F_LCD);
  
  int i;
  
  for (i=0;i<=SingleplLeftPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[0][i] = SingleplLeftPal[i];
  for (i=0;i<=SingleplMidPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[1][i] = SingleplMidPal[i];
  for (i=0;i<=SingleplRightPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[2][i] = SingleplRightPal[i];
  for (i=0;i<=SingleplRightPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[3][i] = PlayerRPal[i];
	
  vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
  
  u16* SingleplLeft = oamAllocateGfx(&oamMain, SpriteSize_32x16, SpriteColorFormat_256Color);
  u16* SingleplMid = oamAllocateGfx(&oamMain, SpriteSize_32x16, SpriteColorFormat_256Color);
  u16* SingleplRight = oamAllocateGfx(&oamMain, SpriteSize_32x16, SpriteColorFormat_256Color);
  u16* PlayerR = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
  
  dmaCopy(SingleplLeftTiles, SingleplLeft, SingleplLeftTilesLen);
  dmaCopy(SingleplMidTiles, SingleplMid, SingleplMidTilesLen);
  dmaCopy(SingleplRightTiles, SingleplRight, SingleplRightTilesLen);
  dmaCopy(PlayerRTiles, PlayerR, PlayerRTilesLen);
  
  Spr1X = 32;
  Spr2X = 64;
  Spr3X = 96;
  PlY = 64;
  
  timerStart(0, ClockDivider_256, TIMER_FREQ(490), timer);
  
  lcdMainOnBottom();
  
  while(1)
  {
    oamSet(&oamMain,
	        0, //SpriteNumber (0-127)
			Spr1X, Spr1Y, //Sprite X and Y variables)
			0, //Priority (0 render last)
			0, //Palete number if Multiple Paletes
			SpriteSize_32x16,
			SpriteColorFormat_256Color,
			SingleplLeft, //Pointer to the graphic
			-1, //Sprite rotation data
			false, //double the size when rotating?
			false, //hide the sprite?
			false, false, //vflip?, hflip?
			false //apply mosaic?
			);
	oamSet(&oamMain,
	        1,
			Spr2X, Spr2Y,
			0,
			1,
			SpriteSize_32x16,
			SpriteColorFormat_256Color,
			SingleplMid,
			-1,
			false,
			false,
			false, false,
			false
			);
	oamSet(&oamMain,
	        2,
			Spr3X, Spr3Y,
			0,
			2,
			SpriteSize_32x16,
			SpriteColorFormat_256Color,
			SingleplRight,
			-1,
			false,
			false,
			false, false,
			false
			);
	oamSet(&oamMain,
	        3,
			PlX, PlY,
			0,
			3,
			SpriteSize_32x64,
			SpriteColorFormat_256Color,
			PlayerR,
			-1,
			false,
			false,
			false, false,
			false
			);
			
	swiWaitForVBlank();
	oamUpdate(&oamMain);
   }
}
		