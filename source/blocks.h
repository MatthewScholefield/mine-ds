#pragma once
#include "sounds.h"
#include "blockID.h"
#define NUM_SPRITE_BLOCKS 14

int sapling(int leafID);
int leaf(int saplingID);
bool canBreak(int blockID);
bool canDropItem(int blockID);
int getSurvivalItem(int blockID);
int displayBlock(int blockID);
bool isAGroundBlock(int a);
short getLightAmount(int a);
bool isBlockALightSource(int a);
bool item(int a);
bool alwaysRenderBright(int a);
bool blockCastsShadow(int blockID);
void initBlockProperties();
int getHardness(int blockID);
int getType(int blockID);
bool isSpriteBlock(int block);
bool isGrassBlock(int blockID);
SoundAudio getBlockAudio(int blockID);
bool isPlaceableItem(int blockID);
int getGenericBlock(int blockID);
bool shouldRender(int blockID);
bool isFoodStuff(int blockID);
int getFoodValue(int blockID);
int fuelAmount(int blockID);
bool perpetualUpdates(int blockID);

inline bool isBlockWalkThrough(int blockID)
{
	switch (blockID)
	{
	case AIR:
	case YELLOW_FLOWER:
	case RED_FLOWER:
	case SNOW_TOP:
	case TORCH:
	case LADDER:
	case SHRUB:
	case TALL_GRASS:
	case MUSHROOM_BROWN:
	case MUSHROOM_RED:
	case SAPLING_JUNGLE:
	case SAPLING_OAK:
	case SAPLING_SPRUCE:
	case DOOR_OPEN_TOP:
	case DOOR_OPEN_BOTTOM:
	case WATER:
		return true;

	default:
		return false;
	}
}