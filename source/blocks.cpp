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

int hardness[NUM_BLOCKS]; //Slot is ID number, negative number means tool
int blockType[NUM_BLOCKS]; //Type of block/tool
int foodItems[] = {PORKCHOP_RAW, 3, BEEF_RAW, 3, FLESH, 4, BEEF_COOKED, 8, PORKCHOP_COOKED, 8, CHICKEN_RAW, 2, CHICKEN_COOKED, 6};

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

bool isSpriteBlock(int block)
{
	switch (block)
	{
	case TORCH:
	case GLASS:
	case SNOW_TOP:
	case LADDER:
	case MUSHROOM_BROWN:
	case MUSHROOM_RED:
	case SHRUB:
	case TALL_GRASS:
	case FLOWER_RED:
	case FLOWER_YELLOW:
	case SAPLING_JUNGLE:
	case SAPLING_OAK:
	case SAPLING_SPRUCE:
	case CHEST:
	case DOOR_CLOSED_BOTTOM:
	case DOOR_CLOSED_TOP:
		return true;

	default:
		return false;
	}
}

bool isBlockWalkThrough(int blockID)
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
	case TYPE_SOIL:
		return SOUND_GRASS;
	case TYPE_WOOD:
		return SOUND_WOOD;
	case TYPE_STONE:
		return SOUND_STONE;
	default:
		return SOUND_STONE;
	}
}

bool item(int blockID)
{
	switch (blockID)
	{
	case PORKCHOP_RAW:
	case BEEF_RAW:
	case LEATHER:
	case PICKAXE_WOOD:
	case PICKAXE_STONE:
	case PICKAXE_IRON:
	case PICKAXE_GOLD:
	case PICKAXE_DIAMOND:
	case COAL:
	case INGOT_IRON:
	case INGOT_GOLD:
	case DIAMOND:
	case STICK:
	case FLESH:
	case BEEF_COOKED:
	case PORKCHOP_COOKED:
	case CHICKEN_RAW:
	case CHICKEN_COOKED:
	case SHOVEL_DIAMOND:
	case SHOVEL_WOOD:
	case SHOVEL_STONE:
	case SHOVEL_IRON:
	case SHOVEL_GOLD:
	case AXE_DIAMOND:
	case AXE_WOOD:
	case AXE_STONE:
	case AXE_IRON:
	case AXE_GOLD:
	case SWORD_DIAMOND:
	case SWORD_WOOD:
	case SWORD_STONE:
	case SWORD_IRON:
	case SWORD_GOLD:
	case SEEDS_PUMPKIN:
	case SEEDS_WHEAT:
	case BUCKET_EMPTY:
		return true;

	default:
		return false;
	}
}

bool isPlaceableItem(int blockID)
{
	switch (blockID)
	{
	case DOOR_ITEM:
	case BUCKET_EMPTY:
	case BUCKET_WATER:
		return true;

	default:
		return false;
	}
}

bool alwaysRenderBright(int blockID)
{
	switch (blockID)
	{
	case AIR:
	case LOG_OAK:
	case LOG_SPRUCE:
	case LOG_BIRCH:
	case LEAVES_OAK:
	case YELLOW_FLOWER:
	case RED_FLOWER:
	case CACTUS:
	case TORCH:
	case LEAVES_SPRUCE:
	case GLASS:
	case SHRUB:
	case TALL_GRASS:
	case MUSHROOM_RED:
	case MUSHROOM_BROWN:
	case PUMPKIN:
	case SAPLING_JUNGLE:
	case SAPLING_OAK:
	case SAPLING_SPRUCE:
		return true;

	default:
		return false;
	}
}

bool blockCastsShadow(int blockID)
{
	return !isBlockWalkThrough(blockID) && !isSpriteBlock(blockID);
}

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
	switch (blockID)
	{
	case TORCH:
	case PUMPKIN_LIGHT:
	case GLOWSTONE:
	case FURNACE_LIT:
		return true;

	default:
		return false;
	}
}

int getLightAmount(int blockID)
{ //15 - value
	switch (blockID)
	{
	case TORCH:
		return 1;
	case PUMPKIN_LIGHT:
		return 0;
	case GLOWSTONE:
		return 0;
	case FURNACE_LIT:
		return 0;

	default:
		return -1;
	}
}

void initBlockProperties()
{
	//Tools
	setArray(blockType, TYPE_PICKAXE, 5, PICKAXE_STONE, PICKAXE_IRON, PICKAXE_GOLD, PICKAXE_WOOD, PICKAXE_DIAMOND); //Pickaxes
	setArray(blockType, TYPE_SHOVEL, 5, SHOVEL_STONE, SHOVEL_IRON, SHOVEL_GOLD, SHOVEL_WOOD, SHOVEL_DIAMOND); //Shovels
	setArray(blockType, TYPE_AXE, 5, AXE_STONE, AXE_IRON, AXE_GOLD, AXE_WOOD, AXE_DIAMOND); //Axes
	setArray(blockType, TYPE_SWORD, 5, SWORD_STONE, SWORD_IRON, SWORD_GOLD, SWORD_WOOD, SWORD_DIAMOND); //Swords

	//Blocks
	setArray(blockType, TYPE_WOOD, 7, LOG_OAK, LOG_JUNGLE, LOG_BIRCH, LOG_SPRUCE, PLANKS_WOOD, CHEST, LADDER); //Axe Blocks
	setArray(blockType, TYPE_SOIL, 7, GRASS_JUNGLE, GRASS, DIRT, SAND, GRAVEL, SNOW_GRASS, MYCELIUM); //Shovel Blocks
	setArray(blockType, TYPE_STONE, 8, STONE, SANDSTONE, COBBLESTONE, COAL_ORE, IRON_ORE, GOLD_ORE, DIAMOND_ORE, BEDROCK); //Blocks that must be mined with a pickaxe

	int i;
	for (i = 1; i < NUM_BLOCKS; ++i)
	{
		switch (blockType[i])
		{
		case TYPE_WOOD:
			hardness[i] = 10;
			break; //Wood hardness is 10
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
	hardness[COBBLESTONE] = 25;
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
	if (getType(blockID) == TYPE_STONE)
	{
		if (getType(getHandID()) != TYPE_PICKAXE)
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
				if (getHandID() == PICKAXE_WOOD) return false;
				break;
			case GOLD_ORE:
				if (getHandID() == PICKAXE_WOOD || getHandID() == PICKAXE_STONE)return false;
				break;
			case DIAMOND_ORE:
				if (getHandID() != PICKAXE_DIAMOND && getHandID() != PICKAXE_IRON) return false;
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

int getGenericBlock(int blockID)
{
	switch (blockID)
	{
	case DOOR_OPEN_TOP:
	case DOOR_OPEN_BOTTOM:
	case DOOR_CLOSED_TOP:
	case DOOR_CLOSED_BOTTOM:
		return DOOR_ITEM;
	}
	return blockID;
}

int getSurvivalItem(int blockID)
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
	default:
		break;
	}
	return getGenericBlock(blockID);
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

bool shouldRender(int blockID)
{
	switch (blockID)
	{
	case WATER:
		return false; 
	default:
		return true;
	}
}
int getFoodValue(int blockID)
{
	for (unsigned int i = 0; i < sizeOfArray(foodItems); i += 2)
	{
		if (foodItems[i] == blockID)
			return foodItems[i+1];
	}
	return 0;
}
bool isFoodStuff(int blockID)
{
	switch(blockID)
	{
		case PORKCHOP_RAW:
		case BEEF_RAW: 
		case FLESH:
		case BEEF_COOKED:
		case PORKCHOP_COOKED:
		case CHICKEN_RAW:
		case CHICKEN_COOKED:
			return true;
			break;	
	}
	return false;
}

int fuelAmount(int blockID)
{
	switch (blockID)
	{
	case COAL:
		return 24;
	case STICK:
		return 1;
	case PLANKS_WOOD:
		return 4;
	case LOG_BIRCH:
	case LOG_OAK:
	case LOG_SPRUCE:
	case LOG_JUNGLE:
		return 16;
	case PICKAXE_WOOD:
	case AXE_WOOD:
		return 4 * 3 + 2;
	case SWORD_WOOD:
		return 2 * 3 + 1;
	case DOOR_ITEM:
		return 4 * 6;
	case LADDER:
		return 8;
	default:
		return 0;
	}
}

bool perpetualUpdates(int blockID)
{
	switch (blockID)
	{
	case FURNACE:
	case FURNACE_LIT:
		return true;
	default:
		return false;
	}
}