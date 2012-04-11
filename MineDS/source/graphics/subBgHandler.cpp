#include <nds.h>
#include "font.h"
#include "sub_bg.h"
uint16 *bgptr;
uint16 *fontgfx;
inline void setSubTileXY(int x, int y, uint16 tile,int palette,int flip)
{
	tile |= palette<<12;
	tile |= flip<<10;
	//bgptr[(x%64) + (y%64)*64] = tile;
	bgptr[x + y*32] = tile;
}
void setSubBgTile(int x,int y,int tile)
{
	const int palette=0;
	//x=x*2; //Convert tiles of 16 to tiles of 8
	//y=y*2;
	//tile=tile*4;
	//Draw the 4 (8x8) tiles
	setSubTileXY(x,y,tile,palette,0);
	//setSubTileXY(x+1,y,tile+2,palette);
	//setSubTileXY(x,y+1,tile+1,palette);
	//setSubTileXY(x+1,y+1,tile+3,palette);
}
void setSubBgTile(int x,int y,int tile,int flip)
{
	const int palette=0;
	//x=x*2; //Convert tiles of 16 to tiles of 8
	//y=y*2;
	//tile=tile*4;
	//Draw the 4 (8x8) tiles
	setSubTileXY(x,y,tile,palette,flip);
	//setSubTileXY(x+1,y,tile+2,palette);
	//setSubTileXY(x,y+1,tile+1,palette);
	//setSubTileXY(x+1,y+1,tile+3,palette);
}
void subBgInit()
{
	videoSetModeSub(MODE_5_2D | DISPLAY_BG_EXT_PALETTE);	
	vramSetBankC(VRAM_C_SUB_BG); 
	int bg = bgInitSub(2,  BgType_ExRotation, BgSize_ER_256x256, 1,6); //16bit BG
	bgptr=bgGetMapPtr(bg);
	fontgfx=(uint16*)0x0620400;
	vramSetBankH(VRAM_H_LCD);
	dmaCopy(&fontPal, VRAM_H_EXT_PALETTE[0][0], fontPalLen); //Copy the palette
	dmaCopy(&sub_bgPal, VRAM_H_EXT_PALETTE[2][0], sub_bgPalLen); //Copy the palette
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	dmaCopy(&sub_bgTiles, bgGetGfxPtr(bg), sub_bgTilesLen);
	dmaCopy(&fontTiles, fontgfx, fontTilesLen);
}
