#include "blockID.h"
#include "blockPages.h"
#include "inventory.h"
#include "graphics/Button.h"
#include "graphics/UI.h"
#include "mining.h"
#include "mainGame.h"
#include "general.h"
#include <nds.h>

const int BLOCK_PAGES [NUM_BLOCK_PAGES][NUM_INV_SPACES] = {
	{BLACK_WOOL, RED_WOOL, DARK_GREEN_WOOL, BROWN_WOOL, BlUE_WOOL, PURPLE_WOOL, CYAN_WOOL, GRAY_WOOL, WHITE_WOOL, DARK_GRAY_WOOL, PINK_WOOL, LIGHT_GREEN_WOOL, YELLOW_WOOL, LIGHT_BLUE_WOOL, MAGENTA_WOOL, ORANGE_WOOL},
	{DIRT, GRASS, SNOW_GRASS, MYCELIUM, STONE, COBBLESTONE, BEDROCK, GRAVEL, SAND, SANDSTONE, LOG_OAK, LOG_SPRUCE, LOG_BIRCH, LOG_JUNGLE, PLANKS_WOOD, LEAVES_OAK, LEAVES_SPRUCE, LEAVES_JUNGLE, GLOWSTONE},
	{CRAFTING_TABLE, LADDER, CHEST, STICK, FURNACE, FURNACE_LIT, GLASS, TNT, TORCH, DOOR_ITEM, BUCKET_WATER,BUCKET_EMPTY},
	{CACTUS, TALL_GRASS, SHRUB, SAPLING_OAK, SAPLING_JUNGLE, SAPLING_SPRUCE, FLOWER_RED, FLOWER_YELLOW, MUSHROOM_RED, MUSHROOM_BROWN, LEAVES_OAK, LEAVES_JUNGLE, LEAVES_SPRUCE, MUSHROOM_STEM, MUSHROOM_TOP, PUMPKIN, PUMPKIN_LIGHT, SEEDS_PUMPKIN, SEEDS_WHEAT},
	{COAL_ORE, IRON_ORE, GOLD_ORE, DIAMOND_ORE, REDSTONE_ORE, INGOT_GOLD, INGOT_IRON, COAL, DIAMOND, IRON_BLOCK, GOLD_BLOCK, DIAMOND_BLOCK},
	{PICKAXE_DIAMOND, PICKAXE_GOLD, PICKAXE_IRON, PICKAXE_STONE, PICKAXE_WOOD, AXE_DIAMOND, AXE_GOLD, AXE_IRON, AXE_STONE, AXE_WOOD, SWORD_DIAMOND, SWORD_GOLD, SWORD_IRON, SWORD_STONE, SWORD_WOOD, SHOVEL_DIAMOND, SHOVEL_GOLD, SHOVEL_IRON, SHOVEL_STONE, SHOVEL_WOOD}
};
int blockPage = 0;

const char *getPageName()
{
	switch (blockPage)
	{
	case PAGE_WOOL: return "Wool";
	case PAGE_BLOCKS: return "Blocks";
	case PAGE_CRAFTING: return "Crafting";
	case PAGE_PLANTS: return "Plants";
	case PAGE_ORES: return "Ores";
	case PAGE_TOOLS: return "Tools";
	default:
		return "Error";
	}
}

void updatePageName()
{
	if (isSurvival())
		return;
	printXY(1, 15, "        ");
	printXY(1, 15, getPageName());
}

void setBlockPage(int page)
{
	if (page < 0)
		page = NUM_BLOCK_PAGES - 1;
	else if (page >= NUM_BLOCK_PAGES)
		page = 0;
	blockPage = page;
	clearInventory();
	for (int i = 0; i < NUM_INV_SPACES; ++i)
		addInventory(BLOCK_PAGES[page][i]);
	updatePageName();
}

void changeBlockPage(bool forward, bool skipUpdate)
{
	blockPage += forward ? 1 : -1;
	if (blockPage < 0)
		blockPage = NUM_BLOCK_PAGES - 1;
	else if (blockPage >= NUM_BLOCK_PAGES)
		blockPage = 0;
	clearInventory();
	for (int i = 0; i < NUM_INV_SPACES; ++i)
		addInventory(BLOCK_PAGES[blockPage][i], 1);
	if (!skipUpdate)
		updatePageName();
}