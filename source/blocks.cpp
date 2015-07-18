/*
		-----------------------------------------
		|               Blocks.cpp              |
		|        General Block Handling         |
		-----------------------------------------
 */
#include "blockID.h"
#include "mining.h"
#include "blocks.h"
#include "mainGame.h"
#include "inventory.h"
#include "graphics/graphics.h"
#include "sounds.h"
#include <stdio.h>
#include <stdarg.h>
#define sizeOfArray(x) (sizeof(x)/sizeof(x[0]))

int walkThroughBlocks[] = {AIR, YELLOW_FLOWER, RED_FLOWER, SNOW_TOP, TORCH, LADDER, SHRUB, TALL_GRASS, MUSHROOM_BROWN, MUSHROOM_RED, SAPLING_JUNGLE, SAPLING_OAK, SAPLING_SPRUCE,DOOR_OPEN_TOP,DOOR_OPEN_BOTTOM};
int renderBright[] = {AIR, LOG_OAK, LOG_SPRUCE, LOG_BIRCH, LEAVES_OAK, YELLOW_FLOWER, RED_FLOWER, CACTUS, TORCH, LEAVES_SPRUCE, GLASS, SHRUB, TALL_GRASS, MUSHROOM_RED, MUSHROOM_BROWN, PUMPKIN, SAPLING_JUNGLE, SAPLING_OAK, SAPLING_SPRUCE};
int lightSourceBlocks[] = {TORCH, PUMPKIN_LIGHT, GLOWSTONE, FURNACE_LIT};
int lightSourceBlocksAmmount[sizeOfArray(lightSourceBlocks)] = {1, 0, 0, 0}; // The Number is equal to 15 - minecraftlightemitvalue
int items[] = {PORKCHOP_RAW, BEEF_RAW, LEATHER, PICKAXE_WOOD, PICKAXE_STONE,
	PICKAXE_IRON, PICKAXE_GOLD, PICKAXE_DIAMOND, COAL, INGOT_IRON, INGOT_GOLD, DIAMOND,
	STICK, FLESH, BEEF_COOKED, PORKCHOP_COOKED, CHICKEN_RAW, CHICKEN_COOKED, SHOVEL_DIAMOND,
	SHOVEL_WOOD, SHOVEL_STONE, SHOVEL_IRON, SHOVEL_GOLD, AXE_DIAMOND, AXE_WOOD, AXE_STONE,
	AXE_IRON, AXE_GOLD, SWORD_DIAMOND, SWORD_WOOD, SWORD_STONE, SWORD_IRON, SWORD_GOLD,
	SEEDS_PUMPKIN, SEEDS_WHEAT};
int hardness[NUM_BLOCKS]; //Slot is ID number, negative number means tool
int blockType[NUM_BLOCKS]; //Type of block/tool
int spriteBlocks[NUM_SPRITE_BLOCKS] = {TORCH, GLASS, SNOW_TOP, LADDER, MUSHROOM_BROWN, MUSHROOM_RED, SHRUB, TALL_GRASS, FLOWER_RED, FLOWER_YELLOW, SAPLING_JUNGLE, SAPLING_OAK, SAPLING_SPRUCE, CHEST};
int foodItems[] = {PORKCHOP_RAW, BEEF_RAW, FLESH, BEEF_COOKED, PORKCHOP_COOKED, CHICKEN_RAW, CHICKEN_COOKED};
int blockItems[] = {DOOR_ITEM};


void setArray(int * array, int setValue, int numOfItems, ...)
{
	//va_list is fine to use and it shouldn't add any overhead as it is used in (i)printf to create infinite arguments.
	va_list slots;
	va_start(slots, numOfItems);
	for (int a = 0; a < numOfItems; ++a)
		array[va_arg(slots, int)] = setValue;
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

int getSpriteBlock(int index)
{
	return spriteBlocks[index];
}

int getSpriteIndex(int block)
{
	int returnVal = -1;
	for (int i = 0; i < NUM_SPRITE_BLOCKS; ++i)
		if (spriteBlocks[i] == block)
		{
			returnVal = i;
			break;
		}
	return returnVal;
}

bool isSpriteBlock(int block)
{
	for (int i = 0; i < NUM_SPRITE_BLOCKS; ++i)
		if (block == spriteBlocks[i])
			return true;
	return false;
}

bool isBlockWalkThrough(int blockID)
{
	int i;
	for (i = 0; (unsigned) i < sizeOfArray(walkThroughBlocks); ++i)
		if (walkThroughBlocks[i] == blockID) return true;
	return false;
}

int sapling(int leafID)
{
	switch (leafID)
	{
		case LEAVES_OAK:
			return SAPLING_OAK;
		case LEAVES_JUNGLE:
			return SAPLING_JUNGLE;
		case LEAVES_SPRUCE:
			return SAPLING_SPRUCE;
		default:
			return BLOCK_DEBUG; //Random block
	}
}

bool isSapling(int blockID)
{
	return leaf(blockID) != BLOCK_DEBUG;
}

int leaf(int saplingID)
{
	switch (saplingID)
	{
		case SAPLING_OAK:
			return LEAVES_OAK;
		case SAPLING_JUNGLE:
			return LEAVES_JUNGLE;
		case SAPLING_SPRUCE:
			return LEAVES_SPRUCE;
		default:
			return BLOCK_DEBUG; //Random block
	}
}

SoundAudio getBlockAudio(int blockID)
{
	switch (blockID)
	{
		case SAND:
			return SOUND_SAND;
		case SNOW_GRASS:
			return SOUND_SNOW;
		case GRAVEL:
			return SOUND_GRAVEL;
		case BLACK_WOOL:
		case RED_WOOL:
		case GREEN_WOOL:
		case BROWN_WOOL:
		case BlUE_WOOL:
		case PURPLE_WOOL:
		case CYAN_WOOL:
		case GRAY_WOOL:
		case WHITE_WOOL:
		case DARK_GRAY_WOOL:
		case PINK_WOOL:
		case LIME_WOOL:
		case YELLOW_WOOL:
		case LIGHT_BLUE_WOOL:
		case MAGENTA_WOOL:
		case ORANGE_WOOL:
			return SOUND_CLOTH;
		case LEAVES_JUNGLE:
		case LEAVES_OAK:
		case LEAVES_SPRUCE:
		case SAPLING_JUNGLE:
		case SAPLING_OAK:
		case SAPLING_SPRUCE:
			return SOUND_GRASS;
		default:
			break;
	}
	switch (blockType[blockID])
	{
		case SOIL:
			return SOUND_GRASS;
		case WOOD:
			return SOUND_WOOD;
		case STONEBLOCK:
			return SOUND_STONE;
		default:
			return SOUND_STONE;
	}
}

bool item(int blockID)
{
	int i;
	for (i = 0; (unsigned) i < sizeOfArray(items); ++i)
		if (items[i] == blockID) return true;
	return false;
}

bool blockItem(int blockID)
{
  int i;
  for (i = 0; (unsigned) i < sizeOfArray(blockItems); ++i)
    if (blockItems[i] == blockID) return true;
  return false;
}

bool alwaysRenderBright(int blockID)
{
	int i;
	for (i = 0; (unsigned) i < sizeOfArray(renderBright); ++i)
		if (renderBright[i] == blockID) return true;
	return false;
}

bool blockCastsShadow(int blockID)
{
	return !isBlockWalkThrough(blockID) && !isSpriteBlock(blockID);
}

/*bool isAGroundBlock(int blockID)
{
	return false;
}*/

bool isGrassBlock(int blockID)
{
	switch (blockID)
	{
		case SNOW_GRASS:
		case GRASS:
		case MYCELIUM:
		case GRASS_JUNGLE:
			return true;
		default:
			return false;
	}
}

bool isBlockALightSource(int blockID)
{
	int i;
	for (i = 0; (unsigned) i < sizeOfArray(lightSourceBlocks); ++i)
		if (lightSourceBlocks[i] == blockID) return true;
	return false;
}

int getLightAmount(int blockID)
{
	int i;
	for (i = 0; (unsigned) i < sizeOfArray(lightSourceBlocks); ++i)
		if (lightSourceBlocks[i] == blockID) return lightSourceBlocksAmmount[i];
	return -1;
}

void initBlockProperties()
{
	//Tools
	setArray(blockType, PICKAXE, 5, PICKAXE_STONE, PICKAXE_IRON, PICKAXE_GOLD, PICKAXE_WOOD, PICKAXE_DIAMOND); //Pickaxes
	setArray(blockType, SHOVEL, 5, SHOVEL_STONE, SHOVEL_IRON, SHOVEL_GOLD, SHOVEL_WOOD, SHOVEL_DIAMOND); //Shovels
	setArray(blockType, AXE, 5, AXE_STONE, AXE_IRON, AXE_GOLD, AXE_WOOD, AXE_DIAMOND); //Axes
	setArray(blockType, SWORD, 5, SWORD_STONE, SWORD_IRON, SWORD_GOLD, SWORD_WOOD, SWORD_DIAMOND); //Swords

	//Blocks
	setArray(blockType, WOOD, 7, LOG_OAK, JUNGLE_WOOD, LOG_BIRCH, LOG_SPRUCE, PLANKS_WOOD, CHEST, LADDER); //Axe Blocks
	setArray(blockType, SOIL, 7, GRASS_JUNGLE, GRASS, DIRT, SAND, GRAVEL, SNOW_GRASS, MYCELIUM); //Shovel Blocks
	setArray(blockType, STONEBLOCK, 8, STONE, SANDSTONE, COBBLESTONE, COAL_ORE, IRON_ORE, GOLD_ORE, DIAMOND_ORE, BEDROCK); //Blocks that must be mined with a pickaxe

	int i;
	for (i = 1; i < NUM_BLOCKS; ++i)
	{
		switch (blockType[i])
		{
			case WOOD:
				hardness[i] = 5;
				break; //Wood hardness is 5
			default:
				hardness[i] = 1;
				break; //Default hardness is 1
		}
		if (item(i))
			hardness[i] = -1;
	}

	//Tool Hardness
	setArray(hardness, -20, 3, PICKAXE_GOLD, AXE_GOLD, SHOVEL_GOLD);
	setArray(hardness, -17, 3, PICKAXE_DIAMOND, AXE_DIAMOND, SHOVEL_DIAMOND);
	setArray(hardness, -12, 3, PICKAXE_IRON, AXE_IRON, SHOVEL_IRON);
	setArray(hardness, -8, 3, PICKAXE_STONE, AXE_STONE, SHOVEL_STONE);
	setArray(hardness, -4, 3, PICKAXE_WOOD, AXE_WOOD, SHOVEL_WOOD);

	//Block Hardness
	setArray(hardness, 2, 5, GRASS_JUNGLE, GRASS, DIRT, SNOW_GRASS, MYCELIUM); //Blocks with DIRT's hardness
	hardness[STONE] = 30;
	hardness[SANDSTONE] = 20;
	hardness[COAL_ORE] = 35;
	hardness[IRON_ORE] = 40;
	hardness[GOLD_ORE] = 40;
	hardness[DIAMOND_ORE] = 45;
	hardness[CHEST] = 20;
}

bool canBreak(int blockID) //checks a blockvalue
{
	if (!isSurvival())
		return true;
	switch (blockID)
	{
		case BEDROCK:
		case AIR:
			return false;
	}
	return true;
}

bool canDropItem(int blockID) //checks is the item should be dropped when mined
{
	if (!isSurvival())
		return false;
	if (getType(blockID) == STONEBLOCK)
	{
		if (getType(getBlockID(getSelectedSlot())) != PICKAXE)
			return false;
		else
		{
			switch (blockID)
			{
				case COAL_ORE:
					blockID = COAL;
					break; //Any pickaxe can break coal
				case STONE:
					blockID = COBBLESTONE;
					break;
				case IRON_ORE:
					if (getBlockID(getSelectedSlot()) == PICKAXE_WOOD) return false;
					break;
				case GOLD_ORE:
					if (getBlockID(getSelectedSlot()) == PICKAXE_WOOD || getBlockID(getSelectedSlot()) == PICKAXE_STONE)return false;
					break;
				case DIAMOND_ORE:
					if (getBlockID(getSelectedSlot()) != PICKAXE_DIAMOND && getBlockID(getSelectedSlot()) != PICKAXE_IRON) return false;
					break;
			}
		}
	}

	switch (blockID)
	{
		case SNOW_TOP:
			return false;
			break; //Can break snow tops, just they won't be added to the inventory
		case AIR:
			return false;
			break;
		case MUSHROOM_STEM:
		case MUSHROOM_TOP:
			return false;
			break;
	}

	return true;
}

int genericBlock(int blockID)
{
	if (!isSurvival())
		return blockID;
	switch (blockID)
	{
		case GRASS:
		case GRASS_JUNGLE:
		case SNOW_GRASS:
		case MYCELIUM:
			return DIRT;
			break;
		case TALL_GRASS:
			return rand() % 3 == 1 ? SEEDS_WHEAT : AIR;
			break;
		case LEAVES_OAK:
		case LEAVES_SPRUCE:
			if (rand() % 3 != 0)
				return AIR;
			else
				return sapling(blockID);
		case LEAVES_JUNGLE:
			if (rand() % 5 != 0)
				return AIR;
			else
				return sapling(blockID);
		case BEDROCK:
			return false;
			break; //Cannot break bedrock
		case SNOW_TOP:
			return AIR;
			break; //Can break snow tops, just they won't be added to the inventory
		case MUSHROOM_STEM:
		case MUSHROOM_TOP:
			return AIR;
			break;
		case COAL_ORE:
			return COAL;
			break; //Any pickaxe can break coal
		case STONE:
			return COBBLESTONE;
			break;
		case IRON_ORE:
			return INGOT_IRON;
			break;
		case GOLD_ORE:
			return INGOT_GOLD;
			break;
		case DIAMOND_ORE:
			return DIAMOND;
			break;
    case DOOR_OPEN_TOP:
    case DOOR_OPEN_BOTTOM:
    case DOOR_CLOSED_TOP:
    case DOOR_CLOSED_BOTTOM:
      return DOOR_ITEM;
      break;
	}
	return blockID;
}

int displayBlock(int blockID)
{
	if (isSapling(blockID))
		return leaf(blockID);
	return blockID;
}

int getType(int blockID)
{
	return blockType[blockID];
}

int getHardness(int blockID)
{
	return hardness[blockID];
}
