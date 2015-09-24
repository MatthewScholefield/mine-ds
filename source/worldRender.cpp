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
#define MAX_BLOCK_SPRITES 100

int sunlight;
int xMin, xMax, yMin, yMax;
uint16 *bg2ptr;
BlockSpriteContainer *blockSprites[MAX_BLOCK_SPRITES] = {};
int sunbrightness;

static void drawBlockGraphic(WorldObject &world, int x, int y)
{
	int blockID = world.blocks[x][y];
	int paletteID = (7 * getBrightness(world, x, y)) / 15;
	x *= 16;
	y *= 16;
	x -= world.camX;
	y -= world.camY;
	std::vector<BlockSpriteContainer>::size_type i;
	for (i=0; i<MAX_BLOCK_SPRITES && blockSprites[i];++i)
		if (blockID == blockSprites[i]->blockID && paletteID == blockSprites[i]->sprite.paletteID && !blockSprites[i]->hasBeenRendered)
		{
			blockSprites[i]->draw(x, y);
			return;
		}
	if (i==MAX_BLOCK_SPRITES)
		showError("Too many block sprites");
	else if (!blockSprites[i])
	{
		blockSprites[i] = new BlockSpriteContainer(blockID, paletteID);
		blockSprites[i]->draw(x, y);
	}
}

int getBrightness(WorldObject &world, int x, int y)
{
	int brightness;
	if (world.sun[x][y] + sunbrightness < world.brightness[x][y])
		brightness = world.sun[x][y] + sunbrightness;
	else
		brightness = world.brightness[x][y];
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

void updateBrightnessAround(WorldObject &world, int x, int y)
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
			world.brightness[i][j] = 15;
			world.sun[i][j] = 15;
			world.lightemit[i][j] = 0;
			if (isBlockALightSource(world.blocks[i][j]))
				world.lightemit[i][j] = 1 + getLightAmount(world.blocks[i][j]);
		}
	}
	//Now update the brightness'
	for (x > 15 ? i = x - 15 : i = 0; x < WORLD_WIDTH - 16 ? i <= x + 16 : i <= WORLD_WIDTH; ++i)
	{
		startshade = false;
		for (j = 0; j <= WORLD_HEIGHT; ++j)
		{
			if (world.brightness[i][j] < 15)
				brightnessSpread(&world, i, j, world.brightness[i][j]/*+ (isBlockWalkThrough(world.blocks[i+1][j]) ? 1:3)*/);
			if (blockCastsShadow(world.blocks[i][j]) && !startshade)
			{
				//	world.lightemit[i][j]=1+sunlight;
				//	world.sun[i][j]=true; // This is a block that is lit by the sun...
				//world.brightness[i][j]=0; //Will be set later
				sunSpread(&world, i, j, sunlight);
				startshade = true;
			}

			else if (!startshade) world.sun[i][j] = sunlight;
			if (!startshade && world.blocks[i][j] != AIR)
				world.sun[i][j] = sunlight;
			else if (!startshade && world.blocks[i - 1][j] == AIR && world.blocks[i + 1][j] == AIR && i != 0) world.sun[i][j] = sunlight;
			if (world.lightemit[i][j] != 0)
			{
				int light = world.lightemit[i][j] - 1;
				brightnessSpread(&world, i, j, light);
			}
			if (world.brightness[i][j] == 16) world.brightness[i][j] = 15;
		}
	}
}

void Calculate_Brightness(WorldObject &world)
{
	int i, j;
	//Kill Every block so it has no brightness...
	for (i = 0; i <= WORLD_WIDTH; ++i)
	{
		for (j = 0; j <= WORLD_HEIGHT; ++j)
		{
			world.brightness[i][j] = 15;
			world.sun[i][j] = 15;
			world.lightemit[i][j] = 0;
			if (isBlockALightSource(world.blocks[i][j]))
				world.lightemit[i][j] = 1 + getLightAmount(world.blocks[i][j]);
		}
	}
	for (i = 0; i <= WORLD_WIDTH; ++i)
	{

		bool startshade = false;
		for (j = 0; j <= WORLD_HEIGHT; ++j)
		{
			if (world.brightness[i][j] < 15)
				brightnessSpread(&world, i, j, world.brightness[i][j]/*+ (isBlockWalkThrough(world.blocks[i+1][j]) ? 1:3)*/);
			if (blockCastsShadow(world.blocks[i][j]) && !startshade)
			{
				//	world.lightemit[i][j]=1+sunlight;
				//	world.sun[i][j]=true; // This is a block that is lit by the sun...
				//world.brightness[i][j]=0; //Will be set later
				sunSpread(&world, i, j, sunlight);
				startshade = true;
			}

			else if (!startshade) world.sun[i][j] = sunlight;
			if (!startshade && world.blocks[i][j] != AIR)
				world.sun[i][j] = sunlight;
			else if (!startshade && world.blocks[i - 1][j] == AIR && world.blocks[i + 1][j] == AIR && i != 0) world.sun[i][j] = sunlight;
			if (world.lightemit[i][j] != 0)
			{
				int light = world.lightemit[i][j] - 1;
				brightnessSpread(&world, i, j, light);
			}
			if (world.brightness[i][j] == 16) world.brightness[i][j] = 15;
		}
	}
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

void renderBlock(WorldObject &world, int i, int j, int blockId, bool add = false)
{
	if (world.sun[i][j] + sunbrightness < world.brightness[i][j])
	{
		int brightness = world.sun[i][j] + sunbrightness + (add ? 6 : 0);
		if (brightness > 15) brightness = 15;
		if (brightness < 0) brightness = 0;
		renderTile16(i, j, blockId, brightness);
	}
	else
		renderTile16(i, j, blockId, world.brightness[i][j] + (add ? 6 : 0));
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
					drawBlockGraphic(world, i, j);
					renderBlock(world, i, j, world.bgblocks[i][j], !alwaysRenderBright(world.bgblocks[i][j]));
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
	for (int i=0; i<MAX_BLOCK_SPRITES && blockSprites[i];++i)
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
	if (y > 0 && world.blocks[x][y-1]==WATER)
	{
		//waterLevel = 16;
	}
	drawRect(Color{
		{r, g, b}}, x * 16 - world.camX, y * 16 - world.camY + 16, 16, -waterLevel);
}

void worldRender_RenderWater(WorldObject &world)
{
	for (int i = world.camX / 16 - 2; i <= world.camX / 16 + 20; ++i)
		for (int j = world.camY / 16 - 20; j <= world.camY / 16 + 20; ++j)
		{
			if (world.blocks[i][j] == WATER)
			{
				if (onScreen(16, i, j, 1, 1))
				{
					renderWater(world, i, j);
				}
				/*if (j < WORLD_WIDTH && (world.blocks[i][j + 1] == AIR || (world.blocks[i][j + 1] == WATER && (getWaterLevel(world, i, j + 1) < MAX_WATER_LEVEL))))
				{
					createWaterMob(i, j, world.data[i][j]);
					world.blocks[i][j] = AIR;
				}*/
			}
		}
}
