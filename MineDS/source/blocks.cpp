/*
		-----------------------------------------
		|               Blocks.cpp              |
		|        General Block Handling         |
		-----------------------------------------
*/
#include "blockID.h"
#include "mining.h"
#include <stdio.h>
#include <stdarg.h>
#define sizeOfArray(x) (sizeof(x)/sizeof(x[0]))

int walkThroughBlocks[]={AIR,YELLOW_FLOWER,RED_FLOWER,SNOW_TOP,TORCH,LADDER,SHRUB,TALL_GRASS,MUSHROOM_BROWN,MUSHROOM_RED};
int renderBright[]={AIR,LOG,OAK_WOOD,BIRCH_WOOD,LEAF,YELLOW_FLOWER,RED_FLOWER,CACTUS,TORCH,REDWOOD_LEAF,GLASS,SHRUB,TALL_GRASS,MUSHROOM_RED,MUSHROOM_BROWN};
int lightSourceBlocks[]={TORCH,PUMPKIN_LIGHT,GLOWSTONE};
int lightSourceBlocksAmmount[sizeOfArray(lightSourceBlocks)]={1,0,0}; // The Number is equal to 15 - minecraftlightemitvalue
int items[]={PORKCHOP_RAW,BEEF_RAW,LEATHER,PICKAXE_WOOD,PICKAXE_STONE,
		PICKAXE_IRON,PICKAXE_GOLD,PICKAXE_DIAMOND,COAL,INGOT_IRON,INGOT_GOLD,DIAMOND,
		STICK,FLESH,BEEF_COOKED,PORKCHOP_COOKED,CHICKEN_RAW,CHICKEN_COOKED,SHOVEL_DIAMOND,
		SHOVEL_WOOD,SHOVEL_STONE,SHOVEL_IRON,SHOVEL_GOLD,AXE_DIAMOND,AXE_WOOD,AXE_STONE,
		AXE_IRON,AXE_GOLD,SWORD_DIAMOND,SWORD_WOOD,SWORD_STONE,SWORD_IRON,SWORD_GOLD,
		SEEDS_PUMPKIN,SEEDS_WHEAT};
int hardness[NUM_BLOCKS]; //Slot is ID number, negative number means tool
int blockType[NUM_BLOCKS]; //Type of block/tool

void setArray(int * array, int setValue, int numOfItems, ...)
{
	//va_list is fine to use and it shouldn't add any overhead as it is used in (i)printf to create infinite arguments.
	va_list slots;
	va_start(slots,numOfItems);
	for (int a = 0; a < numOfItems; a++)
	{
		array[va_arg(slots,int)] = setValue;
	}
	va_end(slots);
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

bool isBlockWalkThrough(int blockID)
{
	int i;
	for(i=0;(unsigned)i<sizeOfArray(walkThroughBlocks);i++)
		if (walkThroughBlocks[i]==blockID) return true;
	return false;
}
bool item(int blockID)
{
	int i;
	for(i=0;(unsigned)i<sizeOfArray(items);i++)
		if (items[i]==blockID) return true;
	return false;
}
bool alwaysRenderBright(int blockID)
{
	int i;
	for(i=0;(unsigned)i<sizeOfArray(renderBright);i++)
		if (renderBright[i]==blockID) return true;
	return false;
}
bool isAGroundBlock(int blockID)
{
	return false;
}
bool isBlockALightSource(int blockID)
{
	int i;
	for(i=0;(unsigned)i<sizeOfArray(lightSourceBlocks);i++)
		if (lightSourceBlocks[i]==blockID) return true;
	return false;
}
int getLightAmount(int blockID)
{
	int i;
	for(i=0;(unsigned)i<sizeOfArray(lightSourceBlocks);i++)
		if (lightSourceBlocks[i]==blockID) return lightSourceBlocksAmmount[i];
	return -1;
}
void initBlockProperties()
{
	//Tools
	setArray(blockType,PICKAXE,5,PICKAXE_STONE, PICKAXE_IRON, PICKAXE_GOLD, PICKAXE_WOOD,PICKAXE_DIAMOND); //Pickaxes
	setArray(blockType,SHOVEL,5,SHOVEL_STONE, SHOVEL_IRON, SHOVEL_GOLD, SHOVEL_WOOD,SHOVEL_DIAMOND); //Shovels
	setArray(blockType,AXE,5,AXE_STONE, AXE_IRON, AXE_GOLD, AXE_WOOD,AXE_DIAMOND); //Axes
	setArray(blockType,SWORD,5,SWORD_STONE, SWORD_IRON, SWORD_GOLD, SWORD_WOOD,SWORD_DIAMOND); //Swords

	//Blocks
	setArray(blockType,WOOD,3,JUNGLE_WOOD,BIRCH_WOOD,OAK_WOOD); //Axe Blocks
	setArray(blockType,SOIL,7,JUNGLE_GRASS,GRASS,DIRT,SAND,GRAVEL,SNOW_GRASS,MYCELIUM);//Shovel Blocks
	setArray(blockType,STONEBLOCK,7,STONE,SANDSTONE,COBBLESTONE,COAL_ORE,IRON_ORE,GOLD_ORE,DIAMOND_ORE); //Blocks that must be mined with a pickaxe

	int i;
	for (i=1;i<=NUM_BLOCKS;i++)
	{
		switch (blockType[i])
		{
		case WOOD: hardness[i]=5; break; //Wood hardness is 5
		default: hardness[i]=1; break; //Default hardness is 1
		}
		if (item(i))
			hardness[i]=-1;
	}

	//Tool Hardness
	setArray(hardness,-20,3,PICKAXE_GOLD,AXE_GOLD,SHOVEL_GOLD);
	setArray(hardness,-17,3,PICKAXE_DIAMOND,AXE_DIAMOND,SHOVEL_DIAMOND);
	setArray(hardness,-12,3,PICKAXE_IRON,AXE_IRON,SHOVEL_IRON);
	setArray(hardness, -8,3,PICKAXE_STONE,AXE_STONE,SHOVEL_STONE);
	setArray(hardness, -4,3,PICKAXE_WOOD,AXE_WOOD,SHOVEL_WOOD);

	//Block Hardness
	setArray(hardness, 2, 5, JUNGLE_GRASS,GRASS,DIRT,SNOW_GRASS,MYCELIUM); //Blocks with DIRT's hardness
	hardness[STONE]=30;
	hardness[SANDSTONE]=20;
	hardness[COAL_ORE]=35;
	hardness[IRON_ORE]=40;
	hardness[GOLD_ORE]=40;
	hardness[DIAMOND_ORE]=45;
}
int getType(int blockID)
{
	return blockType[blockID];
}
int getHardness(int blockID)
{
	return hardness[blockID];
}
