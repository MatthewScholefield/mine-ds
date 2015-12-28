#include "blockID.h"
#include "localizations/locale.h"
#include <stdio.h>
#include <nds.h>
#include <string>

const char *getName(int blockID)
{
	//const char *name;
	
	char buf[20];
	sprintf(buf,"%d",blockID);
	std::string tagname = "block-" + std::string(buf);
	
	return locale(tagname.c_str());
	
	/*
	switch (blockID)
	{
	case -1:
		return "";
	case AIR:
		name = "";
		break;
	case STONE:
		name = "Stone";
		break;
	case DIRT:
		name = "Dirt";
		break;
	case GRASS:
		name = "Grass";
		break;
	case TORCH:
		name = "Torch";
		break;
	case PUMPKIN_LIGHT:
		name = "Pumpkin Light";
		break;
	case GLOWSTONE:
		name = "Glowstone";
		break;
	case LOG_OAK:
		name = "Log";
		break;
	case LOG_SPRUCE:
		name = "Oak Wood";
		break;
	case LOG_BIRCH:
		name = "Birch Wood";
		break;
	case PLANKS_WOOD:
		name = "Plank";
		break;
	case LEAVES_OAK:
		name = "Leaves";
		break;
	case LEAVES_SPRUCE:
		name = "Redwood Leaves";
		break;
	case LEAVES_JUNGLE:
		name = "Jungle Leaves";
		break;
	case GLASS:
		name = "Glass";
		break;
	case CACTUS:
		name = "Cactus";
		break;
	case SAND:
		name = "Sand";
		break;
	case BEDROCK:
		name = "Bedrock";
		break;
	case SNOW_TOP:
		name = "Snow Top";
		break;
	case SNOW_GRASS:
		name = "Snow Grass";
		break;
	case COAL_ORE:
		name = "Coal Ore";
		break;
	case IRON_ORE:
		name = "Iron Ore";
		break;
	case GOLD_ORE:
		name = "Gold Ore";
		break;
	case DIAMOND_ORE:
		name = "Diamond Ore";
		break;
	case REDSTONE_ORE:
		name = "Redstone Ore";
		break;
	case TNT:
		name = "Tnt";
		break;
	case SANDSTONE:
		name = "Sandstone";
		break;
	case RED_FLOWER:
		name = "Red Flower";
		break;
	case YELLOW_FLOWER:
		name = "Yellow Flower";
		break;
	case IRON_BLOCK:
		name = "Iron Block";
		break;
	case GOLD_BLOCK:
		name = "Gold Block";
		break;
	case DIAMOND_BLOCK:
		name = "Diamond Block";
		break;
	case BLACK_WOOL:
		name = "Black Wool";
		break;
	case RED_WOOL:
		name = "Red Wool";
		break;
	case DARK_GREEN_WOOL:
		name = "Dark Green Wool";
		break;
	case BROWN_WOOL:
		name = "Brown Wool";
		break;
	case BlUE_WOOL:
		name = "Blue Wool";
		break;
	case PURPLE_WOOL:
		name = "Purple Wool";
		break;
	case CYAN_WOOL:
		name = "Cyan Wool";
		break;
	case GRAY_WOOL:
		name = "Gray Wool";
		break;
	case WHITE_WOOL:
		name = "White Wool";
		break;
	case DARK_GRAY_WOOL:
		name = "Dark Gray Wool";
		break;
	case PINK_WOOL:
		name = "Pink Wool";
		break;
	case LIGHT_GREEN_WOOL:
		name = "Light Green Wool";
		break;
	case YELLOW_WOOL:
		name = "Yellow Wool";
		break;
	case LIGHT_BLUE_WOOL:
		name = "Light Blue Wool";
		break;
	case MAGENTA_WOOL:
		name = "Fuchsia Wool";
		break;
	case ORANGE_WOOL:
		name = "Orange Wool";
		break;
	case LADDER:
		name = "Ladder";
		break;
	case LOG_JUNGLE:
		name = "Jungle Wood";
		break;
	case GRASS_JUNGLE:
		name = "Jungle Grass";
		break;
	case TALL_GRASS:
		name = "Tall Grass";
		break;
	case SHRUB:
		name = "Shrub";
		break;
	case RAW_PORKCHOP:
		name = "Raw Porkchop";
		break;
	case RAW_BEEF:
		name = "Raw Beef";
		break;
	case LEATHER:
		name = "Leather";
		break;
	case WOOD_PICKAXE:
		name = "Wood Pickaxe";
		break;
	case STONE_PICKAXE:
		name = "Stone Pickaxe";
		break;
	case IRON_PICKAXE:
		name = "Iron Pickaxe";
		break;
	case GOLD_PICKAXE:
		name = "Gold Pickaxe";
		break;
	case DIAMOND_PICKAXE:
		name = "Diamond Pickaxe";
		break;
	case COAL:
		name = "Coal";
		break;
	case INGOT_IRON:
		name = "Ingot Iron";
		break;
	case INGOT_GOLD:
		name = "Ingot Gold";
		break;
	case DIAMOND:
		name = "Diamond";
		break;
	case STICK:
		name = "Stick";
		break;
	case FLESH:
		name = "Flesh";
		break;
	case BEEF_COOKED:
		name = "Beef Cooked";
		break;
	case PORKCHOP_COOKED:
		name = "Porkchop Cooked";
		break;
	case CHICKEN_RAW:
		name = "Chicken Raw";
		break;
	case CHICKEN_COOKED:
		name = "Chicken Cooked";
		break;
	case COBBLESTONE:
		name = "Cobblestone";
		break;
	case FURNACE:
		name = "Furnace";
		break;
	case FURNACE_LIT:
		name = "Furnace Lit";
		break;
	case CRAFTING_TABLE:
		name = "Crafting Table";
		break;
	case CHEST:
		name = "Chest";
		break;
	case SHOVEL_DIAMOND:
		name = "Shovel Diamond";
		break;
	case SHOVEL_WOOD:
		name = "Shovel Wood";
		break;
	case SHOVEL_STONE:
		name = "Shovel Stone";
		break;
	case SHOVEL_IRON:
		name = "Shovel Iron";
		break;
	case SHOVEL_GOLD:
		name = "Shovel Gold";
		break;
	case AXE_DIAMOND:
		name = "Axe Diamond";
		break;
	case AXE_WOOD:
		name = "Axe Wood";
		break;
	case AXE_STONE:
		name = "Axe Stone";
		break;
	case AXE_IRON:
		name = "Axe Iron";
		break;
	case AXE_GOLD:
		name = "Axe Gold";
		break;
	case SWORD_DIAMOND:
		name = "Sword Diamond";
		break;
	case SWORD_WOOD:
		name = "Sword Wood";
		break;
	case SWORD_STONE:
		name = "Sword Stone";
		break;
	case SWORD_IRON:
		name = "Sword Iron";
		break;
	case SWORD_GOLD:
		name = "Sword Gold";
		break;
	case GRAVEL:
		name = "Gravel";
		break;
	case MUSHROOM_BROWN:
		name = "Mushroom Brown";
		break;
	case MUSHROOM_RED:
		name = "Mushroom Red";
		break;
	case MUSHROOM_STEM:
		name = "Mushroom Stem";
		break;
	case MUSHROOM_TOP:
		name = "Mushroom Top";
		break;
	case PUMPKIN:
		name = "Pumpkin";
		break;
	case SEEDS_PUMPKIN:
		name = "Seeds Pumpkin";
		break;
	case SEEDS_WHEAT:
		name = "Seeds Wheat";
		break;
	case MYCELIUM:
		name = "Mycelium";
		break;
	case SAPLING_OAK:
		name = "Oak Sapling";
		break;
	case SAPLING_SPRUCE:
		name = "Spruce Sapling";
		break;
	case SAPLING_JUNGLE:
		name = "Jungle Sapling";
		break;
	case DOOR_ITEM:
		name = "Door";
		break;
	case BUCKET_WATER:
		name = "Water Bucket";
		break;
	case BUCKET_EMPTY:
		name = "Bucket";
		break;
	}
	return name;*/
}

void updateTopName(int blockID)
{
	const int MAX_LENGTH = 20;
	const char *name = getName(blockID);
	int length = strlen(name);
	bool moveUp = length > 12;
	iprintf("\x1b[%d;4H                     \x1b[%d;4H%s", 6 + moveUp, 7 - moveUp, name);
	for (int i = length; i <= MAX_LENGTH; ++i)
		iprintf(" ");
}
