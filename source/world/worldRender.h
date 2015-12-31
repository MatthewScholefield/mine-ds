#pragma once
#include "World.h"
#include "block_small.h"
#include "../graphics/graphics.h"

void worldRender_Init();
void worldRender_Render(World &world);
void worldRender_RenderWater(World &world);
void calculateBrightness(World &world, int leftBound, int rightBound, int bottomBound);
void calculateBrightness(World &world);
void calculateBrightness(World &world, int x, int y);
void setSun(int brightness);
void clearUnusedBlockSprites();

class BlockSpriteContainer
{
public:
	Graphic sprite;
	int blockID;
	bool hasBeenRendered;
	void draw(int x, int y);
	BlockSpriteContainer(int blockID, int paletteID) : sprite(GraphicType::BLOCK, blockID, true, paletteID), blockID(blockID)
	, hasBeenRendered(false) { }

	~BlockSpriteContainer()
	{
	}
};