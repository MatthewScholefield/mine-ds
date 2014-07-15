#include "mining.h" //for NUM_BLOCKS
#include "graphics/graphics.h"
#include "graphics/inventoryGraphics.h"
#include "graphics/subBgHandler.h"
#include "titlescreen.h"
#include "blockID.h"
#include "general.h"
#include "titlescreen.h" // for isSurvival
#include <nds.h> // for keysDown()
#include <stdio.h>
#include "crafting.h"
#include "controls.h"
#include "Config.h"
#include "graphics/Button.h"
recipe craftingRecipes[128];
int nextCraftingRecipe = 0;
Graphic resultBlock;
Graphic neededblocks[4];
int currentViewingRecipe = 0;
Button leftButton(0, 10, "\x011", false);
Button rightButton(29, 10, "\x010", false);
Button backButtonCraftScreen(3, 16, "Back", false);
Button craftButtonCraftScreen(22, 16, "Craft", false);

void craftingInit()
{
	addCraftingRecipe(PLANKS, 4, LOG, 1);
	addCraftingRecipe(PLANKS, 4, JUNGLE_WOOD, 1);
	addCraftingRecipe(PLANKS, 4, BIRCH_WOOD, 1);
	addCraftingRecipe(PLANKS, 4, OAK_WOOD, 1);
	addCraftingRecipe(STICK, 4, PLANKS, 2);
	addCraftingRecipe(PICKAXE_WOOD, 1, PLANK, 3, STICK, 2);
	addCraftingRecipe(PICKAXE_STONE, 1, COBBLESTONE, 3, STICK, 2);
	addCraftingRecipe(PICKAXE_IRON, 1, INGOT_IRON, 3, STICK, 2);
	addCraftingRecipe(PICKAXE_DIAMOND, 1, DIAMOND, 3, STICK, 2);
	addCraftingRecipe(AXE_WOOD, 1, PLANK, 3, STICK, 2);
	addCraftingRecipe(AXE_STONE, 1, COBBLESTONE, 3, STICK, 2);
	addCraftingRecipe(AXE_IRON, 1, INGOT_IRON, 3, STICK, 2);
	addCraftingRecipe(AXE_DIAMOND, 1, DIAMOND, 3, STICK, 2);
	addCraftingRecipe(SWORD_WOOD, 1, PLANK, 2, STICK, 1);
	addCraftingRecipe(SWORD_STONE, 1, COBBLESTONE, 2, STICK, 1);
	addCraftingRecipe(SWORD_IRON, 1, INGOT_IRON, 2, STICK, 1);
	addCraftingRecipe(SWORD_DIAMOND, 1, DIAMOND, 2, STICK, 1);
	addCraftingRecipe(TORCH, 4, COAL, 1, STICK, 1);
	addCraftingRecipe(CRAFTING_TABLE, 1, PLANKS, 4);
	addCraftingRecipe(LADDER, 2, STICK, 8);

	//Furnace Recipes
	addFurnaceRecipe(GLASS, 1, SAND, 1);
	addFurnaceRecipe(BEEF_COOKED, 1, BEEF_RAW, 1);
	addFurnaceRecipe(CHICKEN_COOKED, 1, CHICKEN_RAW, 1);
	addFurnaceRecipe(PORKCHOP_COOKED, 1, PORKCHOP_RAW, 1);
	addFurnaceRecipe(DIAMOND, 1, DIAMOND_ORE, 1);
	addFurnaceRecipe(INGOT_IRON, 1, IRON_ORE, 1);
	loadGraphic(&resultBlock, 0, 0);
	for (int i = 0; i <= 3; i++)
	{
		loadGraphicSub(&neededblocks[i], 2, 0);
	}
}

void addCraftingRecipe(int block, int amount, int aid, int aamount, int bid, int bamount, int cid, int camount, int did, int damount)
{
	craftingRecipes[nextCraftingRecipe].createdblock.blockId = block;
	craftingRecipes[nextCraftingRecipe].createdblock.blockAmount = amount;
	craftingRecipes[nextCraftingRecipe].neededblocks[0].blockId = aid;
	craftingRecipes[nextCraftingRecipe].neededblocks[0].blockAmount = aamount;
	craftingRecipes[nextCraftingRecipe].neededblocks[1].blockId = bid;
	craftingRecipes[nextCraftingRecipe].neededblocks[1].blockAmount = bamount;
	craftingRecipes[nextCraftingRecipe].neededblocks[2].blockId = cid;
	craftingRecipes[nextCraftingRecipe].neededblocks[2].blockAmount = camount;
	craftingRecipes[nextCraftingRecipe].neededblocks[3].blockId = did;
	craftingRecipes[nextCraftingRecipe].neededblocks[3].blockAmount = damount;
	nextCraftingRecipe++;
}

void addFurnaceRecipe(int block, int amount, int aid, int aamount)
{
	addCraftingRecipe(block, amount, aid, aamount, COAL, 1);
}

void updateCraftingGraphics()
{
	unloadGraphic(&resultBlock);
	loadGraphicSub(&resultBlock, 2, craftingRecipes[currentViewingRecipe].createdblock.blockId);
	for (int i = 0; i <= 3; i++)
	{

		unloadGraphic(&neededblocks[i]);
		loadGraphicSub(&neededblocks[i], 2, craftingRecipes[currentViewingRecipe].neededblocks[i].blockId);
	}
}

void craftingMenuInit()
{
	disableInvGraphics();
	lcdMainOnTop();
	consoleClear();
	drawBackground();
	miningSetScene(true);
	backButtonCraftScreen.setVisible(true);
	craftButtonCraftScreen.setVisible(true);
	rightButton.setVisible(true);
	leftButton.setVisible(true);
	currentViewingRecipe = 0;
	updateCraftingGraphics();
}

void craftItem()
{
	if (subInventory(craftingRecipes[currentViewingRecipe].neededblocks[0].blockId, craftingRecipes[currentViewingRecipe].neededblocks[0].blockAmount))
	{
		if (subInventory(craftingRecipes[currentViewingRecipe].neededblocks[1].blockId, craftingRecipes[currentViewingRecipe].neededblocks[1].blockAmount))
		{
			if (subInventory(craftingRecipes[currentViewingRecipe].neededblocks[2].blockId, craftingRecipes[currentViewingRecipe].neededblocks[2].blockAmount))
			{
				if (subInventory(craftingRecipes[currentViewingRecipe].neededblocks[3].blockId, craftingRecipes[currentViewingRecipe].neededblocks[3].blockAmount))
				{
					//can Craft the item....
					addInventory(craftingRecipes[currentViewingRecipe].createdblock.blockId, craftingRecipes[currentViewingRecipe].createdblock.blockAmount);
				}
				else
				{
					addInventory(craftingRecipes[currentViewingRecipe].neededblocks[0].blockId, craftingRecipes[currentViewingRecipe].neededblocks[0].blockAmount);
					addInventory(craftingRecipes[currentViewingRecipe].neededblocks[1].blockId, craftingRecipes[currentViewingRecipe].neededblocks[1].blockAmount);
					addInventory(craftingRecipes[currentViewingRecipe].neededblocks[2].blockId, craftingRecipes[currentViewingRecipe].neededblocks[2].blockAmount);
				}
			}
			else
			{
				addInventory(craftingRecipes[currentViewingRecipe].neededblocks[0].blockId, craftingRecipes[currentViewingRecipe].neededblocks[0].blockAmount);
				addInventory(craftingRecipes[currentViewingRecipe].neededblocks[1].blockId, craftingRecipes[currentViewingRecipe].neededblocks[1].blockAmount);
			}
		}
		else
		{
			addInventory(craftingRecipes[currentViewingRecipe].neededblocks[0].blockId, craftingRecipes[currentViewingRecipe].neededblocks[0].blockAmount);
		}
	}
}

int craftingMenuUpdate(touchPosition* touch, unsigned char* oldX, unsigned char* oldY, unsigned int* oldKeys)
{
	scanKeys();
	showGraphic(&resultBlock, 166, 84);
	iprintf("\x1b[11;23H%d/%d ", checkInventory(craftingRecipes[currentViewingRecipe].createdblock.blockId), craftingRecipes[currentViewingRecipe].createdblock.blockAmount);
	for (int i = 0; i < 4; i++)
	{
		showGraphic(&neededblocks[i], 60, (i % 2 ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2)*8 - 4);
		if (craftingRecipes[currentViewingRecipe].neededblocks[i].blockAmount > 0)
			iprintf("\x1b[%d;%dH%d/%d  ", i % 2 ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2, 10, checkInventory(craftingRecipes[currentViewingRecipe].neededblocks[i].blockId), craftingRecipes[currentViewingRecipe].neededblocks[i].blockAmount);
		else
			iprintf("\x1b[%d;%dH      ", i % 2 ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2, 10);
	}

	if (keysDown() & getControlKey(ACTION_SWITCH_SCREEN) || ((touch->px > (4 - 1)*8 && touch->px < (4 + 5)*8 && touch->py > (17 - 1)*8 && touch->py < (17 + 2)*8) && keysDown() & KEY_TOUCH))
	{

	}
	if (keysHeld() & KEY_TOUCH && !(*oldKeys & KEY_TOUCH))
	{
		touchRead(touch);
		leftButton.setColored(leftButton.isTouching(touch->px, touch->py));
		rightButton.setColored(rightButton.isTouching(touch->px, touch->py));
		backButtonCraftScreen.setColored(backButtonCraftScreen.isTouching(touch->px, touch->py));
		craftButtonCraftScreen.setColored(craftButtonCraftScreen.isTouching(touch->px, touch->py));
	}
	else if (!(keysHeld() & KEY_TOUCH) && *oldKeys & KEY_TOUCH)
	{
		if (rightButton.isTouching(*oldX, *oldY) && rightButton.isColored)
		{
			currentViewingRecipe++;
			if (currentViewingRecipe >= nextCraftingRecipe)
				currentViewingRecipe = 0;
			updateCraftingGraphics();
		}
		else if (leftButton.isTouching(*oldX, *oldY) && leftButton.isColored)
		{
			currentViewingRecipe--;
			if (currentViewingRecipe < 0)
				currentViewingRecipe = nextCraftingRecipe - 1;
			updateCraftingGraphics();
		}
		else if (backButtonCraftScreen.isTouching(*oldX, *oldY) && backButtonCraftScreen.isColored)
		{
			backButtonCraftScreen.setVisible(false);
			craftButtonCraftScreen.setVisible(false);
			leftButton.setVisible(false);
			rightButton.setVisible(false);
			return 1;
		}
		else if (craftButtonCraftScreen.isTouching(*oldX, *oldY) && craftButtonCraftScreen.isColored)
			craftItem();
		leftButton.setColored(false);
		rightButton.setColored(false);
		craftButtonCraftScreen.setColored(false);
		backButtonCraftScreen.setColored(false);
	}
	return 0;
}
