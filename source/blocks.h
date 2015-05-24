#pragma once
#define NUM_SPRITE_BLOCKS 13

int sapling(int leafID);
int leaf(int saplingID);
bool canBreak(int blockID);
bool canDropItem(int blockID);
int genericBlock(int blockID);
int displayBlock(int blockID);
bool isBlockWalkThrough(int a);
bool isAGroundBlock(int a);
int getLightAmount(int a);
bool isBlockALightSource(int a);
bool item(int a);
bool alwaysRenderBright(int a);
void initBlockProperties();
int getHardness(int blockID);
int getType(int blockID);
int getSpriteBlock(int index);
bool isSpriteBlock(int block);
int getSpriteIndex(int block);
bool isGrassBlock(int blockID);