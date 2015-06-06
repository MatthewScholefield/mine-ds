/* _____________________________________________
| 						|
| WorldRender.cpp 				|
| Part of Mine DS , by CoolAs and Ray		|
| Thanks to Dirbaio! 				|
|_______________________________________________|
 */

#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include "blocks.h"
#include "graphics/graphics.h"
#include <math.h>
#define sizeOfArray(x) (sizeof(x)/sizeof(x[0]))
//int execptions[]={AIR};
int sunlight;
int xMin, xMax, yMin, yMax;
uint16 *bg2ptr;
Graphic blockGraphics[NUM_SPRITE_BLOCKS];
int sunbrightness;

int getBrightness(WorldObject* world, int x, int y)
{
	int brightness;
	if (world->sun[x][y] + sunbrightness < world->brightness[x][y])
		brightness = world->sun[x][y] + sunbrightness;
	else
		brightness = world->brightness[x][y];
	if (brightness > 15) brightness = 15;
	else if (brightness < 0) brightness = 0;
	return brightness;
}

void setSun(int brightness)
{
	sunbrightness = brightness;
}

void BlockShader()
{
	//setBackdropColor(RGB15(69,195,237));
	setBackdropColor(RGB15(17, 24, 31));
	vramSetBankE(VRAM_E_LCD);
	//Extreme Thanks to dirbaio...
	for (int i = 1; i < 16; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			uint16 col = VRAM_E_EXT_PALETTE[2][0][j];
			uint16 r = (col >> 0) & 0x1F;
			uint16 g = (col >> 5) & 0x1F;
			uint16 b = (col >> 10) & 0x1F;
			uint16 a = (col >> 15) & 0x1;
			//r = (r - i*2)/2;
			//g = (g - i*2)/2;
			//b = (b - i*2)/2;
			int brightness = (16 - i)*16;
			r = r * brightness / 256;
			g = g * brightness / 256;
			b = b * brightness / 256;
			VRAM_E_EXT_PALETTE[2][i][j] =
					r << 0 |
					g << 5 |
					b << 10 |
					a << 15;

		}
	}
	for (int j = 0; j < 256; ++j)
		VRAM_E_EXT_PALETTE[2][15][j] = 0;
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
}

inline void setTileXY(int x, int y, uint16 tile, int palette)
{
	tile |= palette << 12;
	bg2ptr[(x % 64) + (y % 64)*64] = tile;
}

void brightnessSpread2(WorldObject* world, int x, int y, int brightness)
{
	if (y > WORLD_HEIGHT + 1) return;
	if (y < 0) return;
	if (brightness > 15) return;
	if (world->brightness[x][y] > brightness)
	{
		world->brightness[x][y] = brightness;
		/*if (!isBlockWalkThrough(world->blocks[x][y]))
		{
			brightnessSpread(world,x-1,y,brightness+4);
			brightnessSpread(world,x,y-1,brightness+4);
			brightnessSpread(world,x+1,y,brightness+4);
			brightnessSpread(world,x,y+1,brightness+4);
		}
		else if (isBlockWalkThrough(world->blocks[x][y]))
		{
			brightnessSpread(world,x-1,y,brightness+1);
			brightnessSpread(world,x,y-1,brightness+1);
			brightnessSpread(world,x+1,y,brightness+1);
			brightnessSpread(world,x,y+1,brightness+1);
		}*/
		brightnessSpread2(world, x - 1, y, brightness + (alwaysRenderBright(world->blocks[x - 1][y]) ? 1 : 4));
		brightnessSpread2(world, x, y - 1, brightness + (alwaysRenderBright(world->blocks[x][y - 1]) ? 1 : 4));
		brightnessSpread2(world, x + 1, y, brightness + (alwaysRenderBright(world->blocks[x + 1][y]) ? 1 : 4));
		brightnessSpread2(world, x, y + 1, brightness + (alwaysRenderBright(world->blocks[x][y + 1]) ? 1 : 4));
	}
	else return;
}

void updateBrightnessAround(WorldObject* world, int x, int y)
{
	int i, j;
	bool startshade = false;
	//testfunction();
	//Set all the light values to 0
	//Then set light emit to the approate value.
	for (x > 15 ? i = x - 14 : i = 0; x < WORLD_WIDTH - 16 ? i <= x + 15 : i <= WORLD_WIDTH; ++i)
	{
		startshade = false;
		for (j = 0; j <= WORLD_HEIGHT; ++j)
		{
			world->brightness[i][j] = 15;
			world->sun[i][j] = 15;
			world->lightemit[i][j] = 0;
			if (isBlockALightSource(world->blocks[i][j]))
				world->lightemit[i][j] = 1 + getLightAmount(world->blocks[i][j]);
		}
	}
	//Now update the brightness'
	for (x > 15 ? i = x - 15 : i = 0; x < WORLD_WIDTH - 16 ? i <= x + 16 : i <= WORLD_WIDTH; ++i)
	{
		startshade = false;
		for (j = 0; j <= WORLD_HEIGHT; ++j)
		{
			if (world->brightness[i][j] < 15)
				brightnessSpread(world, i, j, world->brightness[i][j]/*+ (isBlockWalkThrough(world->blocks[i+1][j]) ? 1:3)*/);
			if (!isBlockWalkThrough(world->blocks[i][j]) && !startshade && world->blocks[i][j] != TORCH)
			{
				//	world->lightemit[i][j]=1+sunlight;
				//	world->sun[i][j]=true; // This is a block that is lit by the sun...
				//world->brightness[i][j]=0; //Will be set later
				sunSpread(world, i, j, sunlight);
				startshade = true;
			}

			else if (!startshade) world->sun[i][j] = sunlight;
			if (!startshade && world->blocks[i][j] != AIR)
				world->sun[i][j] = sunlight;
			else if (!startshade && world->blocks[i - 1][j] == AIR && world->blocks[i + 1][j] == AIR && i != 0) world->sun[i][j] = sunlight;
			if (world->lightemit[i][j] != 0)
			{
				int light = world->lightemit[i][j] - 1;
				brightnessSpread(world, i, j, light);
			}
			if (world->brightness[i][j] == 16) world->brightness[i][j] = 15;
		}
	}
}

void Calculate_Brightness(WorldObject* world)
{
	int i, j;
	//Kill Every block so it has no brightness...
	for (i = 0; i <= WORLD_WIDTH; ++i)
	{
		for (j = 0; j <= WORLD_HEIGHT; ++j)
		{
			world->brightness[i][j] = 15;
			world->sun[i][j] = 15;
			world->lightemit[i][j] = 0;
			if (isBlockALightSource(world->blocks[i][j]))
				world->lightemit[i][j] = 1 + getLightAmount(world->blocks[i][j]);
		}
	}
	for (i = 0; i <= WORLD_WIDTH; ++i)
	{

		bool startshade = false;
		for (j = 0; j <= WORLD_HEIGHT; ++j)
		{
			if (world->brightness[i][j] < 15)
				brightnessSpread(world, i, j, world->brightness[i][j]/*+ (isBlockWalkThrough(world->blocks[i+1][j]) ? 1:3)*/);
			if (!isBlockWalkThrough(world->blocks[i][j]) && !startshade && world->blocks[i][j] != TORCH)
			{
				//	world->lightemit[i][j]=1+sunlight;
				//	world->sun[i][j]=true; // This is a block that is lit by the sun...
				//world->brightness[i][j]=0; //Will be set later
				sunSpread(world, i, j, sunlight);
				startshade = true;
			}

			else if (!startshade) world->sun[i][j] = sunlight;
			if (!startshade && world->blocks[i][j] != AIR)
				world->sun[i][j] = sunlight;
			else if (!startshade && world->blocks[i - 1][j] == AIR && world->blocks[i + 1][j] == AIR && i != 0) world->sun[i][j] = sunlight;
			if (world->lightemit[i][j] != 0)
			{
				int light = world->lightemit[i][j] - 1;
				brightnessSpread(world, i, j, light);
			}
			if (world->brightness[i][j] == 16) world->brightness[i][j] = 15;
		}
	}
}
void renderTile16(int a, int b, int c, int d); //HAX

void worldRender_LoadSprites()
{
	for (int i = 0; i < NUM_SPRITE_BLOCKS; ++i)
		loadGraphic(&blockGraphics[i], 2, getSpriteBlock(i));
}

void worldRender_Init(const unsigned int *blockTiles, const unsigned short *blockPalette)
{
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG_EXT_PALETTE;
	bgInit(2, BgType_ExRotation, BgSize_ER_512x512, 0x10, 0);
	bgShow(2);
	bgSetPriority(2, 1);
	REG_BG2CNT |= BG_WRAP_ON;
	bgUpdate();
	vramSetBankE(VRAM_E_LCD);
	dmaCopy(blockPalette, VRAM_E_EXT_PALETTE[2][0], TEXTURE_PAL_LEN); //Copy the palette
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	bg2ptr = bgGetMapPtr(2); //The Map Base
	dmaCopy(blockTiles, bgGetGfxPtr(2), TEXTURE_TILES_LEN); //Copy Tiles
	BlockShader();
	sunlight = 0;
	int i, j;
	for (i = 0; i <= 16; ++i)
		for (j = 0; j <= 16; ++j)
			renderTile16(i, j, AIR, 0);
	sunbrightness = 0;
}

bool onScreen(int SizeMultiplier, int x, int y, int tx, int ty)
{
	int m = SizeMultiplier;
	if (x * m > xMax) return false;
	if (y * m > yMax) return false;
	if (x * m + tx * m < xMin) return false;
	if (y * m + ty * m < yMin) return false;
	return true;
}

void beginRender(int screen_x, int screen_y)
{
	xMin = screen_x;
	yMin = screen_y;
	xMax = screen_x + 256;
	yMax = screen_y + 192;
	bgSetScroll(2, screen_x, screen_y);
	bgUpdate();
}

void renderTile16(int x, int y, int tile, int palette)
{
	x *= 2; //Convert tiles of 16 to tiles of 8
	y *= 2;
	tile *= 4;
	if (palette > 15) palette = 14;
	//Draw the 4 (8x8) tiles
	setTileXY(x, y, tile, palette);
	setTileXY(x + 1, y, tile + 2, palette);
	setTileXY(x, y + 1, tile + 1, palette);
	setTileXY(x + 1, y + 1, tile + 3, palette);
}

void renderBlock(WorldObject* world, int i, int j, int blockId, bool add = false)
{
	if (world->sun[i][j] + sunbrightness < world->brightness[i][j])
	{
		int brightness = world->sun[i][j] + sunbrightness + (add ? 6 : 0);
		if (brightness > 15) brightness = 15;
		if (brightness < 0) brightness = 0;
		renderTile16(i, j, blockId, brightness);
	}
	else
		renderTile16(i, j, blockId, world->brightness[i][j] + (add ? 6 : 0));
}

void renderWorld(WorldObject* world, int screen_x, int screen_y)
{
	int i, j;
	for (i = screen_x / 16 - 2; i <= screen_x / 16 + 20; ++i)
	{
		for (j = screen_y / 16 - 2; j <= screen_y / 16 + 20; ++j)
		{
			//Check The Block is on screen
			if (onScreen(16, i, j, 1, 1))
			{
				if (isSpriteBlock(world->blocks[i][j]) && world->bgblocks[i][j] != AIR
						&& showGraphic(&blockGraphics[getSpriteIndex(world->blocks[i][j])]
						, (i * 16) - screen_x, (j * 16) - screen_y))
					renderBlock(world, i, j, world->bgblocks[i][j], !alwaysRenderBright(world->bgblocks[i][j]));
				else if (world->blocks[i][j] != AIR)
					renderBlock(world, i, j, world->blocks[i][j]);
				else if (world->bgblocks[i][j] != AIR)
					renderBlock(world, i, j, world->bgblocks[i][j], !alwaysRenderBright(world->bgblocks[i][j]));
				else
					renderBlock(world, i, j, AIR);
			}
		}
	}
}

void worldRender_Render(WorldObject* world, int screen_x, int screen_y)
{
	//iprintf("%d,%d\n",screen_x,screen_y);
	beginRender(screen_x, screen_y);
	renderWorld(world, screen_x, screen_y);
}
