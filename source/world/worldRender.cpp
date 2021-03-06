#include <nds.h>
#include <stdio.h>
#include "World.h"
#include "worldRender.h"
#include "../blockID.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../general.h"
#include "../blockUpdaters/WaterUpdater.h"
#include "../graphics/3DHandler.h"
#include "../mobs/mobHandler.h"
#include <math.h>
#include <vector>

int sunlight;
int xMin, xMax, yMin, yMax;
uint16 *bg2ptr;
std::vector<BlockSpriteContainer> blockSprites;
int sunBrightness;

static bool drawBlockGraphic(World &world, int x, int y)
{
	int blockID = world.blocks[x][y];
	int paletteID = (7 * world.brightness[x][y]) / 15;
	x *= 16;
	y *= 16;
	x -= world.camX;
	y -= world.camY;
	for (auto &i : blockSprites)
		if (i.blockID == blockID && i.sprite.paletteID == paletteID)
		{
			i.draw(x, y);
			return true;
		}
	blockSprites.emplace_back(blockID, paletteID);
	blockSprites.back().draw(x, y);
	return true;
}

inline void setTileXY(int x, int y, uint16 tile, int palette)
{
	tile |= palette << 12;
	bg2ptr[(x % 64) + (y % 64)*64] = tile;
}

const int SUB_AMOUNT[16] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3};

void brightnessUpdate(World &world, int x, int y, int brightness)
{
	if ((unsigned) x >= World::WIDTH || (unsigned) y >= World::HEIGHT)
		return;
	int before = world.brightness[x][y];
	int after = max(before, brightness);
	if (before != after)
	{
		world.brightness[x][y] = after;
		int give = after - (isBlockWalkThrough(world.blocks[x][y]) ? 1 : SUB_AMOUNT[after]);
		brightnessUpdate(world, x + 1, y, give);
		brightnessUpdate(world, x - 1, y, give);
		brightnessUpdate(world, x, y + 1, give);
		brightnessUpdate(world, x, y - 1, give);
	}
}

void checkBlock(World &world, int x, int y)
{
	int brightness = world.brightness[x][y] - (isBlockWalkThrough(world.blocks[x][y]) ? 1 : SUB_AMOUNT[world.brightness[x][y]]);
	brightnessUpdate(world, x + 1, y, brightness);
	brightnessUpdate(world, x - 1, y, brightness);
	brightnessUpdate(world, x, y + 1, brightness);
	brightnessUpdate(world, x, y - 1, brightness);
}

void calculateBrightness(World &world, int leftBound, int rightBound, int topBound, int bottomBound)
{
	const int MAX_SPREAD = 7;
	const int MIN_X = max(0, leftBound - MAX_SPREAD);
	const int MAX_X = min(World::WIDTH - 1, rightBound + MAX_SPREAD);
	const int MIN_Y = max(0, topBound - MAX_SPREAD);
	const int MAX_Y = min(World::HEIGHT - 1, bottomBound + MAX_SPREAD);
	for (int i = MIN_X; i <= MAX_X; ++i)
	{
		bool startedShade = false;
		for (int j = 0; j <= MAX_Y; ++j)
		{
			int block = world.blocks[i][j];
			if (!startedShade)
			{
				world.brightness[i][j] = world.sunBrightness;
				if (!isBlockWalkThrough(block))
					startedShade = true;
			}
			else if (block == AIR && world.bgblocks[i][j] == AIR)
				world.brightness[i][j] = world.sunBrightness / 2;
			else
				world.brightness[i][j] = 0;
			if (isBlockALightSource(block))
				world.brightness[i][j] = max(world.brightness[i][j], getLightAmount(block));
		}
	}
	for (int i = MIN_X; i <= MAX_X; ++i)
		for (int j = MIN_Y; j <= MAX_Y; ++j)
			checkBlock(world, i, j);
}

void calculateBrightness(World &world)
{
	calculateBrightness(world, 0, World::WIDTH - 1, 0, World::HEIGHT - 1);
}

void calculateBrightness(World &world, int x, int y)
{
	calculateBrightness(world, x - 8, x + 8, y - 6, y + 6);
}

void renderTile16(int a, int b, int c, int d); //HAX

void worldRender_Init()
{
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	REG_DISPCNT = MODE_5_3D | DISPLAY_BG_EXT_PALETTE;
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
	tile *= 2 * 2;
	if (palette < 0) palette = 1;
	//Draw the 4 (8x8) tiles
	setTileXY(x, y, tile, palette);
	setTileXY(x + 1, y, tile + 2, palette);
	setTileXY(x, y + 1, tile + 1, palette);
	setTileXY(x + 1, y + 1, tile + 3, palette);
}

void renderBlock(World &world, int i, int j, int blockId, bool dim = false)
{
	renderTile16(i, j, blockId, world.brightness[i][j] - (dim ? 6 : 0));
}

void renderWorld(World &world)
{
	int i, j;
	for (i = world.camX / 16 - 2; i <= world.camX / 16 + 20; ++i)
	{
		for (j = world.camY / 16 - 2; j <= world.camY / 16 + 20; ++j)
		{
			//Check The Block is on screen
			if (onScreen(16, i, j, 1, 1))
			{
				if (isSpriteBlock(world.blocks[i][j]) && world.bgblocks[i][j] != AIR)
				{
					if (drawBlockGraphic(world, i, j))
						renderBlock(world, i, j, world.bgblocks[i][j], !alwaysRenderBright(world.bgblocks[i][j]));
					else
						renderBlock(world, i, j, world.blocks[i][j]); //Render sprite block as tile
				}
				else if (world.blocks[i][j] == WATER)
				{
					if (shouldRender(world.bgblocks[i][j]) && !isSpriteBlock(world.bgblocks[i][j]) && world.bgblocks[i][j] != AIR)
						renderBlock(world, i, j, world.bgblocks[i][j], !alwaysRenderBright(world.bgblocks[i][j]));
					else
						renderBlock(world, i, j, AIR);
				}
				else if (shouldRender(world.blocks[i][j]) && world.blocks[i][j] != AIR)
					renderBlock(world, i, j, world.blocks[i][j]);
				else if (shouldRender(world.bgblocks[i][j]) && world.bgblocks[i][j] != AIR)
					renderBlock(world, i, j, world.bgblocks[i][j], !alwaysRenderBright(world.bgblocks[i][j]));
				else
					renderBlock(world, i, j, AIR);
			}
		}
	}
}

void worldRender_Render(World &world)
{
	beginRender(world.camX, world.camY);
	renderWorld(world);
}

void clearUnusedBlockSprites()
{
	for (auto it = blockSprites.begin(); it != blockSprites.end(); ++it)
		if (!it->hasBeenRendered)
		{
			it = blockSprites.erase(it);
			--it;
		}
		else
			it->hasBeenRendered = false;
}

void BlockSpriteContainer::draw(int x, int y)
{
	sprite.draw(x, y, false, 1);
	hasBeenRendered = true;
}

static void renderWater(World &world, int x, int y)
{
	int waterLevel = getWaterLevel(world, x, y);
	int r = 0, g = 255, b = 255;
	if (waterLevel > MAX_WATER_LEVEL) r = 255;
	waterLevel = (waterLevel * 16) / MAX_WATER_LEVEL;
	if (world.bgblocks[x][y] == AIR)
	{
		g /= 2;
		b /= 2;
	}
	drawRect(Color{
		{r, g, b}
	}, x * 16 - world.camX, y * 16 - world.camY + 16, 16, -waterLevel);
}

void worldRender_RenderWater(World &world)
{
	for (int i = world.camX / 16 - 2; i <= world.camX / 16 + 20; ++i)
		for (int j = world.camY / 16 - 20; j <= world.camY / 16 + 20; ++j)
			if (world.blocks[i][j] == WATER)
				if (onScreen(16, i, j, 1, 1))
					renderWater(world, i, j);
}