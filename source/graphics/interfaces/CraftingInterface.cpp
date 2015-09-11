#include "CraftingInterface.h"
#include "../../Recipe.h"
#include "../../inventory.h"
#include "../UI.h"
#include "interfaceHandler.h"

const Recipe CraftingInterface::recipes[NUM_RECIPES] = {
	{PLANKS_WOOD, 4, LOG_OAK, 1},
	{PLANKS_WOOD, 4, LOG_JUNGLE, 1},
	{PLANKS_WOOD, 4, LOG_BIRCH, 1},
	{PLANKS_WOOD, 4, LOG_SPRUCE, 1},
	{STICK, 4, PLANKS_WOOD, 2},
	{PICKAXE_WOOD, 1, PLANKS_WOOD, 3, STICK, 2},
	{PICKAXE_STONE, 1, COBBLESTONE, 3, STICK, 2},
	{PICKAXE_IRON, 1, INGOT_IRON, 3, STICK, 2},
	{PICKAXE_DIAMOND, 1, DIAMOND, 3, STICK, 2},
	{AXE_WOOD, 1, PLANKS_WOOD, 3, STICK, 2},
	{AXE_STONE, 1, COBBLESTONE, 3, STICK, 2},
	{AXE_IRON, 1, INGOT_IRON, 3, STICK, 2},
	{AXE_DIAMOND, 1, DIAMOND, 3, STICK, 2},
	{SWORD_WOOD, 1, PLANKS_WOOD, 2, STICK, 1},
	{SWORD_STONE, 1, COBBLESTONE, 2, STICK, 1},
	{SWORD_IRON, 1, INGOT_IRON, 2, STICK, 1},
	{SWORD_DIAMOND, 1, DIAMOND, 2, STICK, 1},
	{SHOVEL_WOOD, 1, PLANKS_WOOD, 1, STICK, 2},
	{SHOVEL_STONE, 1, COBBLESTONE, 1, STICK, 2},
	{SHOVEL_IRON, 1, INGOT_IRON, 1, STICK, 2},
	{SHOVEL_DIAMOND, 1, DIAMOND, 1, STICK, 2},
	{TORCH, 4, COAL, 1, STICK, 1},
	{CRAFTING_TABLE, 1, PLANKS_WOOD, 4},
	{LADDER, 2, STICK, 8},
	{CHEST, 1, PLANKS_WOOD, 8},
	{DOOR_ITEM, 1, PLANKS_WOOD, 6},
	{BUCKET_EMPTY, 1, INGOT_IRON, 3},
	{FURNACE, 1, COBBLESTONE, 8}
};

void CraftingInterface::updateCraftingGraphics()
{
	unloadGraphic(&resultBlock);
	loadGraphicSub(&resultBlock, GRAPHIC_BLOCK, recipes[page].result.ID);
	iprintf("\x1b[11;22H%d/%d ", checkInventory(recipes[page].result.ID), recipes[page].result.amount);
	for (int i = 0; i <= 3; ++i)
	{
		unloadGraphic(&neededblocks[i]);
		loadGraphicSub(&neededblocks[i], GRAPHIC_BLOCK, recipes[page].needed[i].ID);
		if (recipes[page].needed[i].amount > 0)
			iprintf("\x1b[%d;%dH%d/%d  ", (i % 2) ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2, 10, checkInventory(recipes[page].needed[i].ID), recipes[page].needed[i].amount);
		else
			iprintf("\x1b[%d;%dH      ", (i % 2) ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2, 10);
	}
}

bool CraftingInterface::canCraftRecipe(int recipe)
{
	for (const InvBlock &i : recipes[recipe].needed)
		if (checkInventory(i.ID) < i.amount)
			return false;
	return true;
}

void CraftingInterface::moveCraftingPage(bool right)
{
	int change = right ? 1 : -1;
	int initialPage = page;

	if (!canCraftRecipe(page))
		page += change;
	else
	{
		do
		{
			page += change;
			if (right)
			{
				if (page >= NUM_RECIPES)
					page = 0;
			}
			else if (page < 0)
				page = NUM_RECIPES - 1;
		}
		while (!canCraftRecipe(page) && initialPage != page);
	}
	if (initialPage == page)
		page += change;
	if (page >= NUM_RECIPES)
		page = 0;
	else if (page < 0)
		page = NUM_RECIPES - 1;
	updateCraftingGraphics();
}

void CraftingInterface::craftItem()
{
	if (canCraftRecipe(page))
	{
		for (InvBlock i : recipes[page].needed)
			subInventory(i.ID, i.amount);
		addInventory(recipes[page].result.ID,
					recipes[page].result.amount);
		updateCraftingGraphics();
	}
}

void CraftingInterface::update(WorldObject *world, touchPosition *touch)
{
	showGraphic(&resultBlock, 19 * 8 + 4, 10 * 8 + 4);
	for (int i = 0; i <= 3; ++i)
		showGraphic(&neededblocks[i], 60, ((i % 2) ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2)*8 - 4);
	switch (menu.update(*touch))
	{
	case LEFT:
		moveCraftingPage(false);
		break;
	case RIGHT:
		moveCraftingPage(true);
		break;
	case CRAFT:
		craftItem();
		break;
	case BACK:
		setInterface(INTERFACE_INVENTORY, true);
		break;
	default:
		break;
	}
}

void CraftingInterface::draw()
{
	clearText();
	drawBackground();
	drawBox(5, 8, 22, 7);
	menu.draw();
	updateCraftingGraphics();
}