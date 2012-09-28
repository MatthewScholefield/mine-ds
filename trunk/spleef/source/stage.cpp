#include "stage.h"
#include <nds.h>
#include "blocks.h"
#include "block.h"
int bg;
#define STONE 5
uint16 *bgptr;
void generateNormalStage(Stage* Arena,int type)
{
	int i,j;
	for(i=0;i<=STAGE_WIDTH;i++)
		for (j=0;j<=STAGE_HEIGHT;j++)
		{
			if (i>0 && i < STAGE_WIDTH-1 && j>0 && j<STAGE_HEIGHT-1 )
			{
				if (type==0) Arena->block[i][j]=1;
				else if (type==1) Arena->block[i][j]=3;
				else if (type==2) Arena->block[i][j]=5;
			}
			else Arena->block[i][j]=WOOL;
			
		}
}
inline void setTileXY(int x, int y, uint16 tile)
{
	bgptr[x+y*32] = tile;
}
void renderTile16(int x,int y,int tile)
{
	x=x*2; //Convert tiles of 16 to tiles of 8
	y=y*2;
	tile=tile*4;
	//Draw the 4 (8x8) tiles
	setTileXY(x,y,tile);
	setTileXY(x+1,y,tile+1);
	setTileXY(x,y+1,tile+2);
	setTileXY(x+1,y+1,tile+3);
}
void stageInit()
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);
	dmaCopy(blockPal, BG_PALETTE, sizeof(blockPal));
	dmaCopy(blockTiles, bgGetGfxPtr(bg), blockTilesLen);
	bgptr = bgGetMapPtr(bg);
	int i,j;
	for (i=0;i<=16;i++)
		for(j=0;j<=16;j++) renderTile16(i,j,AIR);
}
void renderStage(Stage* Arena)
{
	int i,j;
	for (i=0;i<=STAGE_WIDTH;i++)
		for(j=0;j<=STAGE_HEIGHT;j++)
		{
			if (i==0) renderTile16(0,j,2);
			else renderTile16(i,j,Arena->block[i][j]);
		}
}
