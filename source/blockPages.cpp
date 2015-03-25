#include "blockID.h"
#include "blockPages.h"
#include "inventory.h"
#include "graphics/Button.h"
#include "graphics/inventoryGraphics.h"
#include "titlescreen.h"
#include "mining.h"
#include "general.h"
#include <nds.h>

const int BLOCK_PAGES [NUM_BLOCK_PAGES][NUM_INV_SPACES] = {
	{BLACK_WOOL, RED_WOOL, DARK_GREEN_WOOL, BROWN_WOOL, BlUE_WOOL, PURPLE_WOOL, CYAN_WOOL, GRAY_WOOL, WHITE_WOOL, DRAK_GRAY_WOOL, PINK_WOOL, LIGHT_GREEN_WOOL, YELLOW_WOOL, LIGHT_BLUE_WOOL, FUCHSIA_WOOL, ORANGE_WOOL},
	{DIRT, GRASS, SNOW_GRASS, MYCELIUM, STONE, COBBLESTONE, SAND, SANDSTONE, LOG_OAK, LOG_SPRUCE, LOG_BIRCH, JUNGLE_WOOD, PLANKS_WOOD, LEAF_OAK, LEAF_SPRUCE, LEAF_JUNGLE, CACTUS},
	{TALL_GRASS, SHRUB, SAPLING_OAK, SAPLING_JUNGLE, SAPLING_SPRUCE, FLOWER_RED, FLOWER_YELLOW, MUSHROOM_RED, MUSHROOM_BROWN},
	{COAL_ORE, IRON_ORE, GOLD_ORE, DIAMOND_ORE, REDSTONE_ORE, INGOT_GOLD, INGOT_IRON, COAL, DIAMOND},
	{PICKAXE_DIAMOND, PICKAXE_GOLD, PICKAXE_IRON, PICKAXE_STONE, PICKAXE_WOOD, AXE_DIAMOND, AXE_GOLD, AXE_IRON, AXE_STONE, AXE_WOOD, SWORD_DIAMOND, SWORD_GOLD, SWORD_IRON, SWORD_STONE, SWORD_WOOD, SHOVEL_DIAMOND, SHOVEL_GOLD, SHOVEL_IRON, SHOVEL_STONE, SHOVEL_WOOD}
};
int blockPage = 0;

Button leftButtonPageScreen(0, 10, "\x011", false);
Button rightButtonPageScreen(29, 10, "\x010", false);
Button backButtonPageScreen(1, 16, "Back", false);

void setBlockPage(int page)
{
	blockPage = page;
	clearInventory();
	for (int i = 0; i < NUM_INV_SPACES; ++i)
		addInventory(BLOCK_PAGES[page][i]);
}

int getBlockPage()
{
	return blockPage;
}

const char *getPageName(int page)
{
	switch (page)
	{
		case PAGE_WOOL: return "Wool";
		case PAGE_BLOCKS: return "Blocks";
		case PAGE_ORES: return "Ores";
		case PAGE_TOOLS: return "Tools";
		default:
			return "Error";
	}
}

void pageMenuInit()
{
	disableInvGraphics();
	lcdMainOnTop();
	consoleClear();
	drawBackground();
	setMiningDisabled(true);
	rightButtonPageScreen.setVisible(true);
	leftButtonPageScreen.setVisible(true);
	backButtonPageScreen.setVisible(true);
	printXY(13, 11, getPageName(getBlockPage()));
}

int pageMenuUpdate(touchPosition* touch, unsigned char* oldX, unsigned char* oldY, unsigned int* oldKeys)
{
	scanKeys();
	if (keysHeld() & KEY_TOUCH && !(*oldKeys & KEY_TOUCH))
	{
		touchRead(touch);
		leftButtonPageScreen.setColored(leftButtonPageScreen.isTouching(touch->px, touch->py));
		rightButtonPageScreen.setColored(rightButtonPageScreen.isTouching(touch->px, touch->py));
		backButtonPageScreen.setColored(backButtonPageScreen.isTouching(touch->px, touch->py));
	}
	else if (!(keysHeld() & KEY_TOUCH) && *oldKeys & KEY_TOUCH)
	{
		if (rightButtonPageScreen.isTouching(*oldX, *oldY) && rightButtonPageScreen.isColored)
		{
			++blockPage;
			if (blockPage >= NUM_BLOCK_PAGES)
				blockPage = 0;
			iprintf("\x1b[11;13H            ");
			printXY(13, 11, getPageName(getBlockPage()));
		}
		else if (leftButtonPageScreen.isTouching(*oldX, *oldY) && leftButtonPageScreen.isColored)
		{
			--blockPage;
			if (blockPage < 0)
				blockPage = NUM_BLOCK_PAGES - 1;
			iprintf("\x1b[11;13H            ");
			printXY(13, 11, getPageName(getBlockPage()));
		}
		else if (backButtonPageScreen.isTouching(*oldX, *oldY) && backButtonPageScreen.isColored)
		{
			backButtonPageScreen.setVisible(false);
			leftButtonPageScreen.setVisible(false);
			rightButtonPageScreen.setVisible(false);
			setBlockPage(blockPage);
			return 1;
		}
		leftButtonPageScreen.setColored(false);
		rightButtonPageScreen.setColored(false);
		backButtonPageScreen.setColored(false);
	}
	return 0;
}