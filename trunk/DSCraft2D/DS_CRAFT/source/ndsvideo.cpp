//This file keeps track of the video on the Nintendo DS, try to keep all of the video in here
//As it makes it easier for a computer port
#include <nds.h> //Include the DS library
#include "block.h" //Include the block graphics
#include "PlayerR.h" //Include the player graphics
#include "player.h" //Include the player functions
#include "sprcount.h" //Include the sprite ID counter
#include "done.h"
#include "mining.h"
#include "world.h"
void setupVideo(){
	//Set Modes and Banks
	videoSetMode(MODE_5_2D| DISPLAY_BG0_ACTIVE);
	videoSetModeSub(MODE_5_2D);
//	vramSetMainBanks(VRAM_A_MAIN_SPRITE_0x06400000,VRAM_B_MAIN_SPRITE_0x06420000,VRAM_C_SUB_BG_0x06200000,VRAM_D_SUB_SPRITE);
	vramSetBankA(VRAM_A_MAIN_SPRITE_0x06400000);
	vramSetBankB(VRAM_B_MAIN_SPRITE_0x06420000);
  	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_SUB_SPRITE);
	vramSetBankG(VRAM_G_MAIN_BG_0x06000000); //Later a smaller one because the tiled BG doesnt need 64 KB, you said? //Great, execpt you can not fit 96KB in a 64KB space :)
	vramSetBankF(VRAM_F_LCD);
	//Copy the Sprite Palettes
	dmaCopy(blockPal,VRAM_F_EXT_SPR_PALETTE[0],blockPalLen);
	dmaCopy(PlayerRPal,VRAM_F_EXT_SPR_PALETTE[1],PlayerRPalLen);
		
	dmaCopy(donePal,VRAM_F_EXT_SPR_PALETTE[2],donePalLen);
	//Set Bank F
  	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
	//Init Sprites
	oamInit(&oamMain,SpriteMapping_1D_128,true);
	//Copy PlayerGraphics
	playerCreateGfx();
	swiWaitForVBlank();
        doneSetup();
	swiWaitForVBlank();
	worldSetUp();
}
//These two functions make it easier to display stuff and gets rid of all the unrememberable stuff like SpriteColorFormat_256Color that we ALWAYS copy-paste.
void createsprite32x64(int x,int y,u16* graphics,bool flipped,int palette){
	if (y<192 && x<256 && x>(-32) && y>(-64)) oamSet(&oamMain,nextSprite(),x,y,0,palette,SpriteSize_32x64,SpriteColorFormat_256Color,graphics,-1,false,false,flipped,false,false); 
	//nextSprite is a call in the sprcount.cpp that returns the next oamID number
} 
void createsprite32x32(int x,int y,u16* graphics,bool flipped,int palette){
	if (y<192 && x<256 && x>(-32) && y>(-32)) oamSet(&oamMain,nextSprite(),x,y,0,palette,SpriteSize_32x32,SpriteColorFormat_256Color,graphics,-1,false,false,flipped,false,false); 
}
void createsprite8x8(int x,int y,u16* graphics,bool flipped,int palette){
	if (y<192 && x<256 && x>(-8) && y>(-8)) oamSet(&oamMain,nextSprite(),x,y,0,palette,SpriteSize_8x8,SpriteColorFormat_256Color,graphics,-1,false,false,flipped,false,false); 
}
