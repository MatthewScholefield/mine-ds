#pragma once
#include "sounds.h"
#define NUM_SPRITE_BLOCKS 14

int sapling(int leafID);
int leaf(int saplingID);
bool canBreak(int blockID);
bool canDropItem(int blockID);
int getSurvivalItem(int blockID);
int displayBlock(int blockID);
bool isBlockWalkThrough(int a);
bool isAGroundBlock(int a);
int getLightAmount(int a);
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