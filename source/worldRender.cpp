#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include "blocks.h"
#include "graphics/graphics.h"
#include "general.h"
#include <math.h>
#include <vector>
#define sizeOfArray(x) (sizeof(x)/sizeof(x[0]))
//int execptions[]={AIR};
int sunlight;
int xMin, xMax, yMin, yMax;
uint16 *bg2ptr;
Graphic blockGraphics[NUM_SPRITE_BLOCKS];
std::vector<BlockSpriteContainer> blockSprites;
int sunbrightness;

static void drawBlockGraphic(WorldObject *world, int x, int y)
{
	int blockID = world->blocks[x][y];
	int paletteID = (7 * getBrightness(world, x, y)) / 15;
	x *= 16;
	y *= 16;
	x -= world->camX;
	y -= world->camY;
	int renderedIndex = -1;
	std::vector<BlockSpriteContainer>::size_type i;
	for (i=0; i<blockSprites.size();++i)
	{
		if (blockID==blockSprites[i].blockID && paletteID==blockSprites[i].sprite.paletteID)
		{
			if (blockSprites[i].hasBeenRendered)
				renderedIndex = i;
			else
			{
				blockSprites[i].draw(x, y);
				break;
			}
		}
	}
	if (i==blockSprites.size())
	{
		if (renderedIndex==-1)
			blockSprites.emplace_back(blockID, paletteID);
		else
		{
			blockSprites.emplace_back();
			blockSprites.back().blockID = blockID;
			setCloneGraphic(&blockSprites[renderedIndex].sprite, &blockSprites.back().sprite);
		}
		blockSprites.back().draw(x, y);
	}
	printXY(1, 4, blockSprites.size());
}

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

inline void setTileXY(int x, int y, uint16 tile, int palette)
{
	tile |= palette << 12;
	bg2ptr[(x % 64) + (y % 64)*64] = tile;
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
			if (blockCastsShadow(world->blocks[i][j]) && !startshade)
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
			if (blockCastsShadow(world->blocks[i][j]) && !startshade)
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
		loadGraphic(&blockGraphics[i], GRAPHIC_BLOCK, getSpriteBlock(i));
}

void worldRender_Init()
{
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG_EXT_PALETTE;
	bgInit(2, BgType_ExRotation, BgSize_ER_512x512, 0x10, 0);
	bgShow(2);
	bgSetPriority(2, 1);
	REG_BG2CNT |= BG_WRAP_ON;
	bgUpdate();
	bg2ptr = bgGetMapPtr(2); //The Map Base
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
				if (isSpriteBlock(world->blocks[i][j]) && world->bgblocks[i][j] != AIR)
				{
					drawBlockGraphic(world, i, j);
					renderBlock(world, i, j, world->bgblocks[i][j], !alwaysRenderBright(world->bgblocks[i][j]));
				}
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
	beginRender(screen_x, screen_y);
	renderWorld(world, screen_x, screen_y);
}


void clearUnusedBlockSprites()
{
	for (std::vector<BlockSpriteContainer>::size_type i=0; i<blockSprites.size();++i)
		if (!blockSprites[i].hasBeenRendered)
		{
			blockSprites.erase(blockSprites.begin() + i);
			--i;
		}
		else
			blockSprites[i].hasBeenRendered = false;
	printXY(1, 1, blockSprites.size());
}

void BlockSpriteContainer::draw(int x, int y)
{
	showGraphic(&sprite, x, y, false, 1);
	printXY(1, 3, x / 16);
	hasBeenRendered = true;
}