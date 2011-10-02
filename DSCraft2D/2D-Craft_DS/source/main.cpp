#include <nds.h>

#include "TitleBottom.h"
#include "TitleTop.h"
#include "Block1.h"
#include "Block2.h"
#include "Block3.h"
#include "PlayerR.h"
#include "PlayerL.h"

int count;
int e = 0;
int Spr1X,Spr2X,Spr1Y,Spr2Y,Spr3X,Spr3Y,PlX,PlY;
int FallSpeed = 0;
bool falling = false;

void timer()
{
	    scanKeys();
		int held = keysHeld();
 
		if( held & KEY_LEFT)
		PlX = PlX -1;
 
		if( held & KEY_RIGHT)
		PlX = PlX +1;
		
		int i;
		
		if( held & KEY_UP)
		{
		 falling = false;
		  for (i = 0; i < 16; i++)
		  {
		   PlY = PlY -1;
			  if (i = 16) 
			  falling = true;
		  }
		}
		
		if (PlY == 256)
		{
		PlY = 0;
		}
			
		if (PlX == Spr1X)
		{
		  if (PlY == Spr1Y)
		  {
		    falling = false;
		  }
		}
		else
		{
		  falling = true;
		  PlY = PlY +FallSpeed;
		  if (FallSpeed == 10)
		  {
		  
		  }
		  else
		  {
		    FallSpeed = FallSpeed +1;
		  }
		}
		
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
  
  for (i=0;i<Block1PalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[0][i] = Block1Pal[i];
  for (i=0;i<=Block2PalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[1][i] = Block2Pal[i];
  for (i=0;i<=Block3PalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[2][i] = Block3Pal[i];
  for (i=0;i<=PlayerRPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[3][i] = PlayerRPal[i];
  for (i=0;i<=PlayerRPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[4][i] = PlayerLPal[i];
	
  vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
  
  u16* Block1 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
  u16* Block2 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
  u16* Block3 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
  u16* PlayerR = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
  u16* PlayerL = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
  
  dmaCopy(Block1Tiles, Block1, Block1TilesLen);
  dmaCopy(Block2Tiles, Block2, Block2TilesLen);
  dmaCopy(Block3Tiles, Block3, Block3TilesLen);
  dmaCopy(PlayerRTiles, PlayerR, PlayerRTilesLen);
  dmaCopy(PlayerLTiles, PlayerL, PlayerLTilesLen);
  
  Spr1X = 32;
  Spr1Y = 128;
  Spr2X = 48;
  Spr2Y = 128;
  Spr3X = 64;
  Spr3Y = 128;
  PlX = 32;
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
			SpriteSize_16x16,
			SpriteColorFormat_256Color,
			Block1, //Pointer to the graphic
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
			SpriteSize_16x16,
			SpriteColorFormat_256Color,
			Block2,
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
			SpriteSize_16x16,
			SpriteColorFormat_256Color,
			Block3,
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
		