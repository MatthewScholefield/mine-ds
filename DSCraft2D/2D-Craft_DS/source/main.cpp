#include <nds.h>
#include "gameshelper.h"
#include "blocks.h"
#include "PlayerR.h"
#include "TitleTop.h"
int x,y,vy;
int gravity=1;
int blockposx[8];
int blockposy[8];
int blocktype[8];
	int framecount=0;
void updategravity(){
	if (vy<20) vy+=gravity; //vy is speed
	bool dofall=true;
	int i;
	scanKeys();
	for(i=0;i<=7;i++)
	{
		if(spritecol(x,y,blockposx[i],blockposy[i],32,64,32,32) && blocktype[i]==0)
		{
			dofall=false;
			y=blockposy[i]-63;
			vy=0;		
		}
		else if (spritecol(x,y,blockposx[i],blockposy[i],32,64,32,32) && blocktype[i]==1)
		{
			vy=-17;
		}
	}
	if (dofall)
	{
			y+=vy;
	}
	else if(keysHeld() & KEY_A)
	{
		//Make our speed a negitive number, this will make the guy go up!
		y=blockposy[i]+64;
		vy=-11; // The -value is the rate which the guy jumps (DONT make it 1 hundred :P OR 1)
	}

	if(keysHeld() & KEY_LEFT)
		x-=2;
	if (keysHeld() & KEY_RIGHT)
		x+=2;
}
void setupOam(){
	oamInit(&oamMain,SpriteMapping_1D_32,true); //Main oam With extended palettes
}
void copypalettes()
{
dmaCopy(blocksPal,VRAM_F_EXT_SPR_PALETTE[0],blocksPalLen); //A Quick way of doing a for loop copy
dmaCopy(PlayerRPal,VRAM_F_EXT_SPR_PALETTE[1],PlayerRPalLen); //A Quick way of doing a for loop copy
}
int main(){

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

    int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    dmaCopy(TitleTopBitmap, bgGetGfxPtr(bg), 256*256);
    dmaCopy(TitleTopPal, BG_PALETTE, 256*2);


	videoSetMode(MODE_5_2D);
	setupOam();
	vramSetBankA(VRAM_A_MAIN_SPRITE);//Make room for Sprites THIS USES A
	vramSetBankF(VRAM_F_LCD);//Unlock VRAM_F
	copypalettes();
  	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);//Then SEt F
	u16* blocks[3]; //4 sprite gfx spaces in a array 
	int i;
	//Allocate memory
	for (i=0;i<=3;i++)
		blocks[i]=oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	u16* player=oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);	
	dmaCopy(PlayerRTiles,player,PlayerRTilesLen);//Copy the player
//	----------------------IMPORTANT CODE BELOW---------------------------
	u8* tile=(u8*)&blocksTiles;
	dmaCopy(blocksTiles,blocks[0],32*32); //Notice the 32*32, Copy only the first 32x32 pixels
						//In the grit file there is a -gB8 line which means 8 bits per pixel this is one byte
						// 32x32 pixels is the same as 32*32 bytes
	tile+=32*32;
	dmaCopy(tile,blocks[1],32*32);//I'm moving the pointer forward 32*32 pixels as well!
	tile+=32*32;
	dmaCopy(tile,blocks[2],32*32);
//	--------------------------END OF IMPORTANT CODE----------------------
	//Set the positions of the blocks
	for (i=0;i<=7;i++)
		blockposy[i]=128;
	//Set the type of the blocks to all 0 (non bouncy)
	for (i=0;i<=7;i++)
		blocktype[i]=0;
	//Make the last block bouncy
	blocktype[7]=1;
	blocktype[2]=2; //make the 3rd block walk through
	blocktype[3]=2; //make the 4ht block walk through
	//Set the x position of the blocks
	blockposx[0]=0;
	blockposx[1]=32;
	blockposx[2]=64;
	blockposx[3]=96;
	blockposx[4]=128;
	blockposx[5]=160;
	blockposx[6]=192;
	blockposx[7]=224;
	swiWaitForVBlank();

	while(1)
	{
		updategravity(); 
		for(i=0;i<=7;i++)//Draw the blocks
			oamSet(&oamMain,i,blockposx[i],blockposy[i],0,0,SpriteSize_32x32,SpriteColorFormat_256Color,blocks[i%3],-1,false,false,false,false,false);
		//Now Draw the Player
		oamSet(&oamMain,8,x,y,0,1,SpriteSize_32x64,SpriteColorFormat_256Color,player,-1,false,y<-64,keysHeld() & KEY_LEFT,false,false); //The keysHeld() & KEY_LEFT Flips the image on the H axis if you press left :) The y<0 part hides the sprite if he jumps too high :)
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		if(y>192) y=0;
		if(x>256)x=0;
		if (x<-32)x=256;
	}
}
