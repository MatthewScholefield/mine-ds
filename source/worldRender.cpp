#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include "blocks.h"
#include "graphics/graphics.h"
#include "general.h"
#include "blockUpdaters/WaterUpdater.h"
#include "graphics/3DHandler.h"
#include "mobs/mobHandler.h"
#include <math.h>
#include <vector>
#define MAX_BLOCK_SPRITES 100

int sunlight;
int xMin, xMax, yMin, yMax;
uint16 *bg2ptr;
BlockSpriteContainer *blockSprites[MAX_BLOCK_SPRITES] = {};
int sunBrightness;

static bool drawBlockGraphic(WorldObject &world, int x, int y)
{
	int blockID = world.blocks[x][y];
	int paletteID = (7 * world.brightness[x][y]) / 15;
	x *= 16;
	y *= 16;
	x -= world.camX;
	y -= world.camY;
	std::vector<BlockSpriteContainer>::size_type i;
	for (i = 0; i < MAX_BLOCK_SPRITES && blockSprites[i]; ++i)
		if (blockID == blockSprites[i]->blockID && paletteID == blockSprites[i]->sprite.paletteID && !blockSprites[i]->hasBeenRendered)
		{
			blockSprites[i]->draw(x, y);
			return true;
		}
	if (i == MAX_BLOCK_SPRITES)
		return false;
	else if (!blockSprites[i])
	{
		blockSprites[i] = new BlockSpriteContainer(blockID, paletteID);
		if (!blockSprites[i]->loaded)
		{
			delete blockSprites[i];
			blockSprites[i] = nullptr;
			return false;
		}
		blockSprites[i]->draw(x, y);
	}
	return true;
}

inline void setTileXY(int x, int y, uint16 tile, int palette)
{
	tile |= palette << 12;
	bg2ptr[(x % 64) + (y % 64)*64] = tile;
}

void brightnessUpdate(WorldObject &world, int x, int y, int brightness)
{
	if ((unsigned) x > WORLD_WIDTH || (unsigned) y > WORLD_HEIGHT)
		return;
	int before = world.brightness[x][y];
	int after = std::max(before, brightness);
	if (before != after)
	{
		world.brightness[x][y] = after;
		int sub;
		if (isBlockWalkThrough(world.blocks[x][y]))
			sub = 1;
		else
		{
			switch (after)
			{
			case 0 ... 4:
				sub = 1;
				break;
			case 5 ... 8:
				sub = 2;
				break;
			case 9 ... 15:
				sub = 3;
				break;
			default:
				sub = 1;
			}
		}
		int give = after - sub;
		brightnessUpdate(world, x + 1, y, give);
		brightnessUpdate(world, x - 1, y, give);
		brightnessUpdate(world, x, y + 1, give);
		brightnessUpdate(world, x, y - 1, give);
	}
}

void checkBlock(WorldObject &world, int x, int y)
{
	int brightness = world.brightness[x][y];
	if (isBlockWalkThrough(world.blocks[x][y]))
		brightness -= 1;
	else
	{
		switch (brightness)
		{
		case 0 ... 4:
			brightness -= 1;
			break;
		case 5 ... 8:
			brightness -= 2;
			break;
		case 9 ... 15:
			brightness -= 3;
			break;
		default:
			brightness -= 1;
		}
	}
	brightnessUpdate(world, x + 1, y, brightness);
	brightnessUpdate(world, x - 1, y, brightness);
	brightnessUpdate(world, x, y + 1, brightness);
	brightnessUpdate(world, x, y - 1, brightness);
}

void calculateBrightness(WorldObject &world, int leftBound, int rightBound, int topBound, int bottomBound)
{
	cpuStartTiming(0);
	const int MAX_SPREAD = 7;
	const int MIN_X = std::max(0, leftBound - MAX_SPREAD);
	const int MAX_X = std::min(WORLD_WIDTH, rightBound + MAX_SPREAD);
	const int MIN_Y = std::max(0, topBound - MAX_SPREAD);
	const int MAX_Y = std::min(WORLD_HEIGHT, bottomBound + MAX_SPREAD);
	for (int i = MIN_X; i <= MAX_X; ++i)
	{
		bool startedShade = false;
		for (int j = 0; j <= MAX_Y; ++j)
		{
			int block = world.blocks[i][j];
			if (startedShade)
				world.brightness[i][j] = isBlockALightSource(block) ? getLightAmount(block) : 0;
			else
			{
				world.brightness[i][j] = world.worldBrightness;
				if (!isBlockWalkThrough(block))
					startedShade = true;
			}
		}
	}
	for (int i = MIN_X; i <= MAX_X; ++i)
		for (int j = MIN_Y; j <= MAX_Y; ++j)
			checkBlock(world, i, j);
	printXY(1, 1, cpuEndTiming());
}

void calculateBrightness(WorldObject &world)
{
	calculateBrightness(world, 0, WORLD_WIDTH, 0, WORLD_HEIGHT);
}

void calculateBrightness(WorldObject &world, int x, int y)
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

void renderBlock(WorldObject &world, int i, int j, int blockId, bool dim = false)
{
	renderTile16(i, j, blockId, world.brightness[i][j] - (dim ? 6 : 0));
}

void renderWorld(WorldObject &world)
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

void worldRender_Render(WorldObject &world)
{
	beginRender(world.camX, world.camY);
	renderWorld(world);
}

void clearUnusedBlockSprites()
{
	for (int i = 0; i < MAX_BLOCK_SPRITES && blockSprites[i]; ++i)
		if (!blockSprites[i]->hasBeenRendered)
		{
			delete blockSprites[i];
			blockSprites[i] = nullptr;
			--i;
		}
		else
			blockSprites[i]->hasBeenRendered = false;
}

void BlockSpriteContainer::draw(int x, int y)
{
	showGraphic(&sprite, x, y, false, 1);
	hasBeenRendered = true;
}

static void renderWater(WorldObject &world, int x, int y)
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

void worldRender_RenderWater(WorldObject &world)
{
	for (int i = world.camX / 16 - 2; i <= world.camX / 16 + 20; ++i)
		for (int j = world.camY / 16 - 20; j <= world.camY / 16 + 20; ++j)
			if (world.blocks[i][j] == WATER)
				if (onScreen(16, i, j, 1, 1))
					renderWater(world, i, j);
}