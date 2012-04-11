/* _____________________________________________
| |
| WorldRender.cpp |
| Part of Mine DS , by CoolAs and Ray |
| Thanks to Dirbaio! |
|_____________________________________________|
*/

#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <math.h>
int sunlight;
int xMin, xMax, yMin, yMax;
uint16 *bg2ptr;
void BlockShader(){
	vramSetBankE(VRAM_E_LCD);
	//Extreme Thanks to dirbaio...
	for(int i = 1; i < 16; i++)
	{
		for(int j = 0; j < 256; j++)
		{
			uint16 col = VRAM_E_EXT_PALETTE[2][0][j];
			uint16 r = (col >> 0) & 0x1F;
			uint16 g = (col >> 5) & 0x1F;
			uint16 b = (col >> 10) & 0x1F;
			uint16 a = (col >> 15) & 0x1;
			//r = (r - i*2)/2;
			//g = (g - i*2)/2;
			//b = (b - i*2)/2;
			int brightness = (16-i)*16;
			r = r*brightness/256;
			g = g*brightness/256;
			b = b*brightness/256;
			VRAM_E_EXT_PALETTE[2][i][j] =
			r << 0 |
			g << 5 |
			b << 10 |
			a << 15;
		}
	}
	for(int j = 0; j < 256; j++)
	{
		VRAM_E_EXT_PALETTE[2][15][j] = 0;
	}
	
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
}
inline void setTileXY(int x, int y, uint16 tile,int palette)
{
	tile |= palette<<12;
	bg2ptr[(x%64) + (y%64)*64] = tile;
}
void brightnessSpread(worldObject* world,int x,int y, int brightness)
{
	if (y>WORLD_HEIGHT+1) return;
	if (y<0) return;
	if (world->brightness[x][y]>brightness)
	{
		world->brightness[x][y]=brightness;
		if (world->blocks[x][y]!=AIR && world->blocks[x][y]!=LEAF && world->blocks[x][y]!=LOG && world->blocks[x][y]!=DARK_WOOD && world->blocks[x][y]!=FLOWER_RED && world->blocks[x][y]!=FLOWER_YELLOW && world->blocks[x][y]!=CACTUS)
		{
			brightnessSpread(world,x-1,y,brightness+4);
			brightnessSpread(world,x,y-1,brightness+4);
			brightnessSpread(world,x+1,y,brightness+4);
			brightnessSpread(world,x,y+1,brightness+4);
		}
		else if (world->blocks[x][y]==AIR || world->blocks[x][y]==LEAF || world->blocks[x][y]==LOG || world->blocks[x][y]==DARK_WOOD || world->blocks[x][y]==FLOWER_RED || world->blocks[x][y]==FLOWER_YELLOW || world->blocks[x][y]==CACTUS)
		{
			brightnessSpread(world,x-1,y,brightness+1);
			brightnessSpread(world,x,y-1,brightness+1);
			brightnessSpread(world,x+1,y,brightness+1);
			brightnessSpread(world,x,y+1,brightness+1);
		}
	}
	else return;
}

void updateBrightnessAround(worldObject* world,int x,int y)
{
	int sx,sy;
	for (sx=x-14;sx<=x+14;sx++)
		for(sy=y-14;sy<=y+14;sy++)
		{
			world->brightness[sx][sy]=15;
		}
	for (x>15 ? sx=x-15 : sx=0;x<WORLD_WIDTH-15 ? sx<=x+15 : sx<=WORLD_WIDTH;sx++)
		for (y>15 ? sy=y-15 : sy=0;y<WORLD_HEIGHT-15 ? sy<=y+15 : sy<=WORLD_HEIGHT;sy++)
		{
			if(world->brightness[sx][sy]!=15)
			{
				int bright=world->brightness[sx][sy]; //HAXXY CODE
				world->brightness[sx][sy]=15;//HAXXY CODE
				brightnessSpread(world,sx,sy,bright);
			}
			if (world->lightemit[sx][sy]!=0)
			{
				int light=world->lightemit[sx][sy]-1;		
				brightnessSpread(world,sx,sy,light);
			}
		}
	
}
void Calculate_Brightness(worldObject* world)
{
	int i,j;
	//Kill Every block so it has no brightness...
	for(i=0;i<=WORLD_WIDTH;i++)
	{
		for (j=0;j<=WORLD_HEIGHT;j++)
		{
		world->brightness[i][j]=15;
		}
	}
	for(i=0;i<=WORLD_WIDTH;i++)
	{

		bool startshade=false;
		//First put sunshine on the top-blocks.
		for (j=0;j<=WORLD_HEIGHT;j++)
		{
			if(world->blocks[i][j]!=AIR && !startshade)
			{
				world->lightemit[i][j]=1+sunlight;
				//world->brightness[i][j]=0; //Will be set later
				startshade=true;
			}
			/*else if(world->blocks[i][j]!=AIR && world->blocks[i][j]!=LEAF && world->blocks[i][j]!=LOG && world->blocks[i][j]!=DARK_WOOD && world->blocks[i][j]!=FLOWER_RED && world->blocks[i][j]!=FLOWER_YELLOW && world->blocks[i][j]!=CACTUS && world->brightness[i][j-1]<=12)
			{
				world->brightness[i][j]=world->brightness[i][j-1]+4;
			}
			else if (world->blocks[i][j]==AIR || world->blocks[i][j]==LEAF || world->blocks[i][j]==LOG || world->blocks[i][j]==DARK_WOOD || world->blocks[i][j]==FLOWER_RED || world->blocks[i][j]==FLOWER_YELLOW || world->blocks[i][j]==CACTUS) world->brightness[i][j]=world->brightness[i][j-1];*/
			if (world->lightemit[i][j]!=0)
			{
				int light=world->lightemit[i][j]-1;
				brightnessSpread(world,i,j,light);
			}
			if(world->brightness[i][j]==16) world->brightness[i][j]=15;
		}
	}
}
void renderTile16(int a,int b, int c, int d); //HAX
void worldRender_Init()
{
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG_EXT_PALETTE;
	bgInit(2, BgType_ExRotation, BgSize_ER_512x512, 0x10, 0);
	bgShow(2);
	REG_BG2CNT |= BG_WRAP_ON;
	bgUpdate();
	vramSetBankE(VRAM_E_LCD);
	dmaCopy(&block_smallPal, VRAM_E_EXT_PALETTE[2][0], block_smallPalLen); //Copy the palette
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	bg2ptr=bgGetMapPtr(2); //The Map Base
	dmaCopy(&block_smallTiles, bgGetGfxPtr(2), block_smallTilesLen); //Copy Tiles
	BlockShader();
	sunlight=0;
	int i,j;
	for (i=0;i<=16;i++)
		for (j=0;j<=16;j++)
			renderTile16(i,j,AIR,0);
}

bool onScreen(int SizeMultiplier,int x,int y, int tx,int ty)
{
    int m = SizeMultiplier;
	if(x * m > xMax) return false;
	if(y * m > yMax) return false;
	if(x * m + tx * m < xMin) return false;
	if(y * m + ty * m < yMin) return false;
	return true;
}

void beginRender(int screen_x,int screen_y)
{
	xMin=screen_x;
	yMin=screen_y;
	xMax=screen_x+256;
	yMax=screen_y+192;
	bgSetScroll (2, screen_x, screen_y);
	bgUpdate();
}

void renderTile16(int x,int y,int tile,int palette)
{
	x=x*2; //Convert tiles of 16 to tiles of 8
	y=y*2;
	tile=tile*4;
	//Draw the 4 (8x8) tiles
	setTileXY(x,y,tile,palette);
	setTileXY(x+1,y,tile+2,palette);
	setTileXY(x,y+1,tile+1,palette);
	setTileXY(x+1,y+1,tile+3,palette);
}

void renderWorld(worldObject* world,int screen_x,int screen_y)
{
	int i,j;
	for(i=screen_x/16-2;i<=screen_x/16+20;i++){
		for(j=screen_y/16-2;j<=screen_y/16+20;j++){
		//Check The Block is on screen
		if(onScreen(16,i,j,1,1))
			{
				renderTile16(i,j,world->blocks[i][j],world->brightness[i][j]);

			}
		}
	}
}

void worldRender_Render(worldObject* world,int screen_x,int screen_y)
{
	if (keysDown() & KEY_L) sunlight++;
	if (keysDown() & KEY_R) sunlight--;
	beginRender(screen_x,screen_y);
	renderWorld(world,screen_x,screen_y);
}
