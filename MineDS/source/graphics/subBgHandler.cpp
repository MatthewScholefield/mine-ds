#include <nds.h>
#include "font.h"
#include "sub_bg.h"
uint16 *bgptr;
uint16 *fontgfx;
#define V_FLIP 2
#define H_FLIP 1
#define BOTH_FLIP 3
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
void drawButton(int x,int y, int sizex)
{
	int i;
	setSubBgTile(x,y,26);
	setSubBgTile(x,y+2,26,V_FLIP);
	setSubBgTile(x+sizex,y,26,H_FLIP);
	setSubBgTile(x+sizex,y+2,26,BOTH_FLIP);
	for(i=0;i<=sizex-1;i++)
		setSubBgTile(x+1+i,y,30);
	for(i=0;i<=sizex-1;i++)
		setSubBgTile(x+1+i,y+2,30,V_FLIP);
	setSubBgTile(x,y+1,27);
	setSubBgTile(x+sizex,y+1,27,H_FLIP);
	for (i=0;i<=sizex-1;i++)
		setSubBgTile(x+1+i,y+1,28 + (i%2));
}
void drawButtonColored(int x,int y, int sizex)
{
	int i;
	setSubBgTile(x,y,58);
	setSubBgTile(x,y+2,58,V_FLIP);
	setSubBgTile(x+sizex,y,58,H_FLIP);
	setSubBgTile(x+sizex,y+2,58,BOTH_FLIP);
	for(i=0;i<=sizex-1;i++)
		setSubBgTile(x+1+i,y,62);
	for(i=0;i<=sizex-1;i++)
		setSubBgTile(x+1+i,y+2,62,V_FLIP);
	setSubBgTile(x,y+1,59);
	setSubBgTile(x+sizex,y+1,59,H_FLIP);
	for(i=0;i<=sizex-1;i++)
		setSubBgTile(x+1+i,y+1,60 + (i%2));
}
