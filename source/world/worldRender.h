#pragma once
#include "WorldObject.h"
#include "block_small.h"
#include "../graphics/graphics.h"

void worldRender_Init();
void worldRender_Render(WorldObject &world);
void worldRender_RenderWater(WorldObject &world);
void calculateBrightness(WorldObject &world, int leftBound, int rightBound, int bottomBound);
void calculateBrightness(WorldObject &world);
void calculateBrightness(WorldObject &world, int x, int y);
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