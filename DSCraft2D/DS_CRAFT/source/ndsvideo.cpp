#include <nds.h>
#include "block.h"
#include "PlayerR.h"
#include "player.h"
#include "sprcount.h"
void setupVideo(){
	//Set Modes and Banks
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankF(VRAM_F_LCD);
	//Copy the Sprite Palettes
	dmaCopy(blockPal,VRAM_F_EXT_SPR_PALETTE[0],blockPalLen);
	dmaCopy(PlayerRPal,VRAM_F_EXT_SPR_PALETTE[1],PlayerRPalLen);
	//Set Bank F
  	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
	//Init Sprites
	oamInit(&oamMain,SpriteMapping_1D_32,true);
	//Copy PlayerGraphics
	playerCreateGfx();
}
void createsprite32x64(int x,int y,u16* graphics,bool flipped,int palette){
	oamSet(&oamMain,nextSprite(),x,y,0,palette,SpriteSize_32x64,SpriteColorFormat_256Color,graphics,-1,false,false,flipped,false,false); 
}
void createsprite32x32(int x,int y,u16* graphics,bool flipped,int palette){
	oamSet(&oamMain,nextSprite(),x,y,0,palette,SpriteSize_32x32,SpriteColorFormat_256Color,graphics,-1,false,false,flipped,false,false); 
}
