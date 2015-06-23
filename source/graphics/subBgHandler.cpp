#include <nds.h>
#include "font.h"
#include "sub_bg.h"
#include "graphics.h"
#include "../fontHandler.h"
#define V_FLIP 2
#define H_FLIP 1
#define BOTH_FLIP 3

uint16 *bgptr;
double subBgCalcX = 0;
double subBgCalcY = 0;
int subBgX = 0;
int subBgY = 0;

inline void setSubTileXY(int x, int y, uint16 tile, int palette, int flip)
{
	tile |= palette << 12;
	tile |= flip << 10;
	//bgptr[(x%64) + (y%64)*64] = tile;
	bgptr[x + y * 32] = tile;
}

void setSubBgTile(int x, int y, int tile)
{
	const int palette = 0;
	//x=x*2; //Convert tiles of 16 to tiles of 8
	//y=y*2;
	//tile=tile*4;
	//Draw the 4 (8x8) tiles
	setSubTileXY(x, y, tile, palette, 0);
	//setSubTileXY(x+1,y,tile+2,palette);
	//setSubTileXY(x,y+1,tile+1,palette);
	//setSubTileXY(x+1,y+1,tile+3,palette);
}

void setSubBgTile(int x, int y, int tile, int flip)
{
	const int palette = 0;
	//x=x*2; //Convert tiles of 16 to tiles of 8
	//y=y*2;
	//tile=tile*4;
	//Draw the 4 (8x8) tiles
	setSubTileXY(x, y, tile, palette, flip);
	//setSubTileXY(x+1,y,tile+2,palette);
	//setSubTileXY(x,y+1,tile+1,palette);
	//setSubTileXY(x+1,y+1,tile+3,palette);
}

void subBgInit()
{
	videoSetModeSub(MODE_5_2D | DISPLAY_BG_EXT_PALETTE);
	REG_BG2CNT |= BG_WRAP_ON;
	bgSetScroll(6, 1, 1);
	vramSetBankC(VRAM_C_SUB_BG);
	int bg = bgInitSub(2, BgType_ExRotation, BgSize_ER_256x256, 1, 6); //16bit BG
	bgptr = bgGetMapPtr(bg);
	vramSetBankH(VRAM_H_LCD);
	dmaCopy(&fontPal, VRAM_H_EXT_PALETTE[0][0], fontPalLen); //Copy the palette
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	dmaCopy(&fontTiles, (uint16*) 0x0620400, fontTilesLen);
}

void moveSubBg(int dX, int dY)
{
	subBgX += dX;
	subBgY += dY;
	subBgX = 0;
	if (subBgY + 192 > 256)
		subBgY = 256 - 192;
	else if (subBgY < 0)
		subBgY = 0;
}

int getScrollY()
{
	return subBgCalcY;
}

void updateSubBG()
{
	subBgCalcX += (double(subBgX) - subBgCalcX)*0.03;
	subBgCalcY += (double(subBgY) - subBgCalcY)*0.03;
	bgSetScroll(6, subBgCalcX, subBgCalcY);
	bgSetScroll(getConsoleID(), subBgCalcX, subBgCalcY);
	bgUpdate();
}

void drawButton(int x, int y, int sizex)
{
	sizex -= 1;
	setSubBgTile(x, y + 1, 27);
	setSubBgTile(x, y, 26);
	setSubBgTile(x, y + 2, 26, V_FLIP);
	setSubBgTile(x + sizex, y + 1, 27, H_FLIP);
	setSubBgTile(x + sizex, y, 26, H_FLIP);
	setSubBgTile(x + sizex, y + 2, 26, BOTH_FLIP);

	int i;
	for (i = 0; i < sizex - 1; ++i)
		setSubBgTile(x + 1 + i, y, 30);
	for (i = 0; i < sizex - 1; ++i)
		setSubBgTile(x + 1 + i, y + 2, 30, V_FLIP);
	for (i = 0; i < sizex - 1; ++i)
		setSubBgTile(x + 1 + i, y + 1, 28 + (i % 2));
}

void drawButtonColored(int x, int y, int sizex)
{
	sizex -= 1;
	setSubBgTile(x, y, 58);
	setSubBgTile(x, y + 2, 58, V_FLIP);
	setSubBgTile(x + sizex, y, 58, H_FLIP);
	setSubBgTile(x + sizex, y + 2, 58, BOTH_FLIP);
	setSubBgTile(x, y + 1, 59);
	setSubBgTile(x + sizex, y + 1, 59, H_FLIP);

	int i;
	for (i = 0; i < sizex - 1; ++i)
		setSubBgTile(x + 1 + i, y, 62);
	for (i = 0; i < sizex - 1; ++i)
		setSubBgTile(x + 1 + i, y + 2, 62, V_FLIP);
	for (i = 0; i < sizex - 1; ++i)
		setSubBgTile(x + 1 + i, y + 1, 60 + (i % 2));
}

void drawBoxCenter(int x, int y, int lx, int ly) //Draws a box without borders
{
	for (int i = 0; i < lx; ++i)
		for (int j = 0; j < ly; ++j)
			setSubBgTile(x + i, y + j, 28 + (i + 1) % 2);
}

void drawBoxFrame(int x, int y, int lx, int ly)
{
	setSubBgTile(x, y, 26); //Top-Left Corner
	setSubBgTile(x, y + ly - 1, 26, V_FLIP); //Bottom-Left Corner
	setSubBgTile(x + lx - 1, y + ly - 1, 26, BOTH_FLIP); //Bottom-Right Corner
	setSubBgTile(x + lx - 1, y + 0, 26, H_FLIP); //Top-Right Corner
	for (int i = 1; i < ly - 1; ++i)
	{
		setSubBgTile(x + lx - 1, y + i, 27, H_FLIP); //Right Edge
		setSubBgTile(x, y + i, 27); //Left Edge
	}
	for (int i = 1; i < lx - 1; ++i)
	{
		setSubBgTile(x + i, y, 30); //Top Edge
		setSubBgTile(x + i, y + ly - 1, 30, V_FLIP); //Bottom Edge
	}
}

void drawBox(int x, int y, int lx, int ly)
{
	drawBoxFrame(x, y, lx, ly);
	drawBoxCenter(x + 1, y + 1, lx - 2, ly - 2);
}
