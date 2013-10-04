/*
        -----------------------------------------
        |               Blocks.cpp              |
        |        General Block Handling         |
        -----------------------------------------
*/
#include "blockID.h"
#include "mining.h"
#include <stdio.h>
//#include <iostream>
#include <initializer_list>
#define sizeOfArray(x) (sizeof(x)/4)

//Block Types
#define SOIL 1 //Any type of Grass and Dirt
#define WOOD 2

//Tool Types
#define PICKAXE -1
#define SHOVEL -2
#define AXE -3
#define SWORD -4

int walkThroughBlocks[]={AIR,YELLOW_FLOWER,RED_FLOWER,SNOW_TOP,TORCH,LADDER,SHRUB,TALL_GRASS};
int renderBright[]={AIR,LOG,OAK_WOOD,BIRCH_WOOD,LEAF,YELLOW_FLOWER,RED_FLOWER,CACTUS,TORCH,REDWOOD_LEAF,GLASS,SNOW_TOP,SHRUB,TALL_GRASS};
int lightSourceBlocks[]={TORCH,PUMPKIN_LIGHT,GLOWSTONE};
int items[]={PORKCHOP_RAW,BEEF_RAW,LEATHER,PICKAXE_WOOD,PICKAXE_STONE,
		PICKAXE_IRON,PICKAXE_GOLD,PICKAXE_DIAMOND,COAL,IRON,GOLD,DIAMOND,
		STICK,FLESH,BEEF_COOKED,PORKCHOP_COOKED,CHICKEN_RAW,CHICKEN_COOKED, SHOVEL_DIAMOND, SHOVEL_WOOD, SHOVEL_STONE, SHOVEL_IRON, SHOVEL_GOLD, AXE_DIAMOND, AXE_WOOD,AXE_STONE,AXE_IRON,AXE_GOLD,SWORD_DIAMOND,SWORD_WOOD,SWORD_STONE,SWORD_IRON,SWORD_GOLD};
int hardness[NUM_BLOCKS]; //Slot is ID number, negative number means tool
int blockType[NUM_BLOCKS]; //Type of block/tool

void setArray(int * array, std::initializer_list<size_t> slots, int setValue) {
    for (auto i : slots) {
        array[i] = setValue;
    }
}
/*
========HARDNESSS========
---Tools----
-Negative value
-Faster Mining = Farther from 0
-Slower Mining = Closer to 0

---Blocks---
-Positive Value
-Harder Object = Higher Value
-Softer Object = Lower value (Min 0)

	 DIRT ~ 1
	STONE ~ 20
TREE WOOD ~ 3

==========TYPE==========
Only used for block category,
NOT for block properties (WalkThrough)
otherwise values could overlap

-----Tools------
-Negative Value-
----------------

-----Blocks-----
-Positive Value-
----------------
*/

#define NUM_OF_LIGHT_SOURCES 3
//The Number is equal to 15 - minecraftlightemitvalue
int sourceAmount[]={1,0,0};

int isBlockWalkThrough(int blockID)
{
        int i;
        bool isWalkThrough=false;
        for(i=0;i<=sizeOfArray(walkThroughBlocks);i++)
        {
                if (walkThroughBlocks[i]==blockID) isWalkThrough=true;
        }
        return isWalkThrough;
}
int item(int blockID)
{
        int i;
        bool isItem=false;
        for(i=0;i<=sizeOfArray(items);i++)
        {
                if (items[i]==blockID) isItem=true;
        }
        return isItem;
}
int alwaysRenderBright(int blockID)
{
        int i;
        bool isWalkThrough=false;
        for(i=0;i<=sizeOfArray(renderBright);i++)
        {
                if (renderBright[i]==blockID) isWalkThrough=true;
        }
        return isWalkThrough;
}
int isAGroundBlock(int blockID)
{
        return false;
}
bool isBlockALightSource(int blockID)
{
	int i;
        bool isLightSource=false;
        for(i=0;i<NUM_OF_LIGHT_SOURCES;i++)
        {
                if (lightSourceBlocks[i]==blockID) isLightSource=true;
        }
        return isLightSource;
}
int getLightAmount(int blockID)
{
	int i;
        bool isLightSource=false;
        for(i=0;i<NUM_OF_LIGHT_SOURCES;i++)
        {
                if (lightSourceBlocks[i]==blockID) return sourceAmount[i];
        }
}
void initHardness()
{
	int i;
	for (i=1;i<=NUM_BLOCKS;i++)
		hardness[i]=1;
	for (i=1;i<=NUM_BLOCKS;i++)
		if (item(i))
			hardness[i]=-2;
	setArray(hardness,{PICKAXE_GOLD,AXE_GOLD,SHOVEL_GOLD},-20);
	setArray(hardness,{PICKAXE_DIAMOND,AXE_DIAMOND,SHOVEL_DIAMOND},-16);
	setArray(hardness,{PICKAXE_IRON,AXE_IRON,SHOVEL_IRON},-10);
	setArray(hardness,{PICKAXE_STONE,AXE_STONE,SHOVEL_STONE},-6);
	setArray(hardness,{PICKAXE_WOOD,AXE_WOOD,SHOVEL_WOOD},-4);

	hardness[STONE]=20;
	hardness[SANDSTONE]=13;
	hardness[BIRCH_WOOD]=3;
	hardness[OAK_WOOD]=3;
	hardness[JUNGLE_WOOD]=3;


}
void initType()
{
	//Tools
	setArray(blockType,{PICKAXE_STONE, PICKAXE_IRON, PICKAXE_GOLD, PICKAXE_WOOD},PICKAXE); //Pickaxes
	setArray(blockType,{SHOVEL_STONE, SHOVEL_IRON, SHOVEL_GOLD, SHOVEL_WOOD},SHOVEL); //Shovels
	setArray(blockType,{AXE_STONE, AXE_IRON, AXE_GOLD, AXE_WOOD},AXE); //Axes
	setArray(blockType,{SWORD_STONE, SWORD_IRON, SWORD_GOLD, SWORD_WOOD},SWORD); //Swords

	//Blocks
	setArray(blockType,{JUNGLE_WOOD,BIRCH_WOOD},WOOD);
	setArray(blockType,{JUNGLE_GRASS,GRASS,DIRT},SOIL);
}
int getType(int blockID)
{
	return blockType[blockID];
}
int getHardness(int blockID)
{
	return hardness[blockID];
}
