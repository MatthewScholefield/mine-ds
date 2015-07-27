#pragma once
#include "world.h"
#include "block_small.h"
#include "graphics/graphics.h"

void worldRender_Init();
void worldRender_Render(WorldObject* world, int screen_x, int screen_y);
void Calculate_Brightness(WorldObject* world);
extern "C"
{
void brightnessSpread(WorldObject* world, int x, int y, int brightness);
void sunSpread(WorldObject* world, int x, int y, int brightness);
}
void updateBrightnessAround(WorldObject* world, int x, int y);
void worldRender_LoadSprites();
void setSun(int brightness);
int getBrightness(WorldObject* world, int x, int y);
void clearUnusedBlockSprites();

class BlockSpriteContainer
{
public:
	Graphic sprite;
	int blockID;
	bool hasBeenRendered;
	void draw(int x, int y);
	BlockSpriteContainer():sprite(),blockID(0),hasBeenRendered(false){}
	BlockSpriteContainer(int blockID, int paletteID):sprite(),blockID(blockID),hasBeenRendered(false)
	{
		loadGraphic(&sprite, GRAPHIC_BLOCK,blockID,16,16,paletteID);
	}
	~BlockSpriteContainer()
	{
		unloadGraphic(&sprite);
	}
};