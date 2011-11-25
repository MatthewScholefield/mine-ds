#include <nds.h>

#include "grass.h" //Background
#include "man.h"   //The player
#include "test.h"  //Test-Player
#include "stick.h" // The Object to pick up

int x1,y1,x2,y2;
int held = keysHeld();

void move(){
  scanKeys();
  held=keysHeld();
    if (held & KEY_LEFT)
     x1-=1;
	if (held & KEY_RIGHT)
     x1+=1;
	if (held & KEY_UP)
     y1-=1;
	if (held & KEY_DOWN)
     y1+=1;
}

int main(void)
{
  videoSetMode(MODE_5_2D);
  vramSetBankD(VRAM_D_MAIN_BG_0x06000000);

  int bg = bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0,0);

  dmaCopy(grassBitmap, bgGetGfxPtr(bg), 256*256);
  dmaCopy(grassPal, BG_PALETTE, 256*2);



  vramSetBankA(VRAM_A_MAIN_SPRITE);
  oamInit(&oamMain,SpriteMapping_1D_32,true);
  vramSetBankF(VRAM_F_LCD);
  
  int i;
  for (i=0;i<=testPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[0][i] = testPal[i];
  for (i=0;i<=testPalLen;i++)
  VRAM_F_EXT_SPR_PALETTE[1][i] = stickPal[i];
  vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
  
  u16* test = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
  u16* stick = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
  dmaCopy(testTiles, test, testTilesLen);
  dmaCopy(stickTiles, stick, stickTilesLen);
  
  x1 = 0;
  y1 = 96;
  
  while(1){
	move(); //To Make the player move you have to call move xD
		//It wont be automatically called xD

  oamSet(&oamMain, //main graphics engine context
    0, //oam index (0 to 127) (spritenumber)
    x1, y1, //x and y pixle location of the sprite
    0, //priority, lower renders last (on top)
    0, //this is the palette index if multiple palettes or the alpha value if bmp sprite
    SpriteSize_32x32,
    SpriteColorFormat_256Color,
    test, //pointer to the loaded graphics
    -1, //sprite rotation data
    false, //double the size when rotating?
    false, //hide the sprite?
    false, false, //vflip, hflip
    false //apply mosaic
  );
  oamSet(&oamMain,
    1,
    x2, y2,
    0,
    1,
    SpriteSize_32x32,
    SpriteColorFormat_256Color,
    stick,
    -1,
    false,
    false,
    false, false,
    false
  );
  
  if (x1<x2+32 && y1+32>=y2 && y1<y2+32 && x1+32>=x2){
    if (x1+32>x2 && x1+32<x2)
	  x1+=2;
	if (x1<x2+32 && x1>x2+32)
	  x1-=2;
	if (y1+32>y2 && y1+32<y2)
      y1+=2;
	if (y1<y2+32 && y1>y2+32)
	  y1-=2;
 }
  swiWaitForVBlank();
  oamUpdate(&oamMain);
  }
  return 0;
}
