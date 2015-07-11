#include "mining.h" //for NUM_BLOCKS
#include "graphics/graphics.h"
#include "graphics/inventoryGraphics.h"
#include "graphics/subBgHandler.h"
#include "graphics/UI.h"
#include "blockID.h"
#include "general.h"
#include "graphics/UI.h" // for isSurvival
#include "inventory.h"
#include <nds.h> // for keysDown()
#include <stdio.h>
#include "crafting.h"
#include "Config.h"
#include "graphics/Button.h"
recipe craftingRecipes[128];
int nextCraftingRecipe = 0;
Graphic resultBlock;
Graphic neededblocks[4];
int currentViewingRecipe = 0;
Button leftButtonCraftScreen(0, 10, "\x011", false);
Button rightButtonCraftScreen(29, 10, "\x010", false);
Button backButtonCraftScreen(3, 16, "Back", false);
Button craftButtonCraftScreen(22, 16, "Craft", false);

void craftingInit()
{
	addCraftingRecipe(PLANKS_WOOD, 4, LOG_OAK, 1);
	addCraftingRecipe(PLANKS_WOOD, 4, JUNGLE_WOOD, 1);
	addCraftingRecipe(PLANKS_WOOD, 4, LOG_BIRCH, 1);
	addCraftingRecipe(PLANKS_WOOD, 4, LOG_SPRUCE, 1);
	addCraftingRecipe(STICK, 4, PLANKS_WOOD, 2);
	addCraftingRecipe(PICKAXE_WOOD, 1, PLANKS_WOOD, 3, STICK, 2);
	addCraftingRecipe(PICKAXE_STONE, 1, COBBLESTONE, 3, STICK, 2);
	addCraftingRecipe(PICKAXE_IRON, 1, INGOT_IRON, 3, STICK, 2);
	addCraftingRecipe(PICKAXE_DIAMOND, 1, DIAMOND, 3, STICK, 2);
	addCraftingRecipe(AXE_WOOD, 1, PLANKS_WOOD, 3, STICK, 2);
	addCraftingRecipe(AXE_STONE, 1, COBBLESTONE, 3, STICK, 2);
	addCraftingRecipe(AXE_IRON, 1, INGOT_IRON, 3, STICK, 2);
	addCraftingRecipe(AXE_DIAMOND, 1, DIAMOND, 3, STICK, 2);
	addCraftingRecipe(SWORD_WOOD, 1, PLANKS_WOOD, 2, STICK, 1);
	addCraftingRecipe(SWORD_STONE, 1, COBBLESTONE, 2, STICK, 1);
	addCraftingRecipe(SWORD_IRON, 1, INGOT_IRON, 2, STICK, 1);
	addCraftingRecipe(SWORD_DIAMOND, 1, DIAMOND, 2, STICK, 1);
	addCraftingRecipe(SHOVEL_WOOD, 1, PLANKS_WOOD, 1, STICK, 2);
	addCraftingRecipe(SHOVEL_STONE, 1, COBBLESTONE, 1, STICK, 2);
	addCraftingRecipe(SHOVEL_IRON, 1, INGOT_IRON, 1, STICK, 2);
	addCraftingRecipe(SHOVEL_DIAMOND, 1, DIAMOND, 1, STICK, 2);
	addCraftingRecipe(TORCH, 4, COAL, 1, STICK, 1);
	addCraftingRecipe(CRAFTING_TABLE, 1, PLANKS_WOOD, 4);
	addCraftingRecipe(LADDER, 2, STICK, 8);
	addCraftingRecipe(CHEST, 1, PLANKS_WOOD, 8);

	//Furnace Recipes
	addFurnaceRecipe(GLASS, 1, SAND, 1);
	addFurnaceRecipe(BEEF_COOKED, 1, BEEF_RAW, 1);
	addFurnaceRecipe(CHICKEN_COOKED, 1, CHICKEN_RAW, 1);
	addFurnaceRecipe(PORKCHOP_COOKED, 1, PORKCHOP_RAW, 1);
	addFurnaceRecipe(DIAMOND, 1, DIAMOND_ORE, 1);
	addFurnaceRecipe(INGOT_IRON, 1, IRON_ORE, 1);
	loadGraphic(&resultBlock, GRAPHIC_PARTICLE, 0);
	for (int i = 0; i <= 3; ++i)
		loadGraphicSub(&neededblocks[i], GRAPHIC_BLOCK, 0);
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
	++nextCraftingRecipe;
}

void addFurnaceRecipe(int block, int amount, int aid, int aamount)
{
	addCraftingRecipe(block, amount, aid, aamount, COAL, 1);
}

static void updateCraftingGraphics()
{
	clearSubGraphics();
	unloadGraphic(&resultBlock);
	loadGraphicSub(&resultBlock, GRAPHIC_BLOCK, craftingRecipes[currentViewingRecipe].createdblock.blockId);
	showGraphic(&resultBlock, 19 * 8 + 4, 10 * 8 + 4);
	iprintf("\x1b[11;22H%d/%d ", checkInventory(craftingRecipes[currentViewingRecipe].createdblock.blockId), craftingRecipes[currentViewingRecipe].createdblock.blockAmount);
	for (int i = 0; i <= 3; ++i)
	{
		unloadGraphic(&neededblocks[i]);
		loadGraphicSub(&neededblocks[i], GRAPHIC_BLOCK, craftingRecipes[currentViewingRecipe].neededblocks[i].blockId);
		showGraphic(&neededblocks[i], 60, ((i % 2) ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2)*8 - 4);
		if (craftingRecipes[currentViewingRecipe].neededblocks[i].blockAmount > 0)
			iprintf("\x1b[%d;%dH%d/%d  ", (i % 2) ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2, 10, checkInventory(craftingRecipes[currentViewingRecipe].neededblocks[i].blockId), craftingRecipes[currentViewingRecipe].neededblocks[i].blockAmount);
		else
			iprintf("\x1b[%d;%dH      ", (i % 2) ? 11 - (i / 2)*2 - 2 : 11 + (i / 2)*2, 10);
	}
	oamUpdate(&oamSub);
}

bool canCraftRecipe(int recipe)
{
	for (InvBlock i : craftingRecipes[recipe].neededblocks)
		if (checkInventory(i.blockId) < i.blockAmount)
			return false;
	return true;
}


void craftingMenuInit()
{
	disableInvGraphics();
	lcdMainOnTop();
	clearText();
	drawBackground();
	setMiningDisabled(true);
	backButtonCraftScreen.setVisible(true);
	craftButtonCraftScreen.setVisible(true);
	rightButtonCraftScreen.setVisible(true);
	leftButtonCraftScreen.setVisible(true);
	drawBox(5, 8, 22, 7);
	for (currentViewingRecipe = 0; !canCraftRecipe(currentViewingRecipe) && currentViewingRecipe < nextCraftingRecipe; ++currentViewingRecipe);
	if (currentViewingRecipe == nextCraftingRecipe)
		currentViewingRecipe = 0;
	updateCraftingGraphics();
}

void craftItem()
{
	if (canCraftRecipe(currentViewingRecipe))
	{
		for (InvBlock i : craftingRecipes[currentViewingRecipe].neededblocks)
			subInventory(i.blockId, i.blockAmount);
		addInventory(craftingRecipes[currentViewingRecipe].createdblock.blockId,
				craftingRecipes[currentViewingRecipe].createdblock.blockAmount, true);
	}
}

int craftingMenuUpdate(touchPosition* touch, unsigned char* oldX, unsigned char* oldY, unsigned int* oldKeys)
{
	
	if (keysHeld() & KEY_TOUCH && !(*oldKeys & KEY_TOUCH))
	{
		touchRead(touch);
		leftButtonCraftScreen.setColored(leftButtonCraftScreen.isTouching(touch->px, touch->py));
		rightButtonCraftScreen.setColored(rightButtonCraftScreen.isTouching(touch->px, touch->py));
		backButtonCraftScreen.setColored(backButtonCraftScreen.isTouching(touch->px, touch->py));
		craftButtonCraftScreen.setColored(craftButtonCraftScreen.isTouching(touch->px, touch->py));
	}
	else if (!(keysHeld() & KEY_TOUCH) && *oldKeys & KEY_TOUCH)
	{
		if (rightButtonCraftScreen.isTouching(*oldX, *oldY) && rightButtonCraftScreen.isColored)
		{
			int initialRecipe = currentViewingRecipe;
			if (!canCraftRecipe(currentViewingRecipe))
				++currentViewingRecipe;
			else
			{
				do
				{
					++currentViewingRecipe;
					if (currentViewingRecipe >= nextCraftingRecipe)
						currentViewingRecipe = 0;
				}
				while (!canCraftRecipe(currentViewingRecipe) && initialRecipe != currentViewingRecipe);
			}
			if (initialRecipe == currentViewingRecipe)
				currentViewingRecipe = initialRecipe + 1;
			if (currentViewingRecipe >= nextCraftingRecipe)
				currentViewingRecipe = 0;
			updateCraftingGraphics();
		}
		else if (leftButtonCraftScreen.isTouching(*oldX, *oldY) && leftButtonCraftScreen.isColored)
		{
			int initialRecipe = currentViewingRecipe;
			if (!canCraftRecipe(currentViewingRecipe))
				--currentViewingRecipe;
			else
			{
				do
				{
					--currentViewingRecipe;
					if (currentViewingRecipe < 0)
						currentViewingRecipe = nextCraftingRecipe - 1;
				}
				while (!canCraftRecipe(currentViewingRecipe) && initialRecipe != currentViewingRecipe);
			}
			if (initialRecipe == currentViewingRecipe)
				currentViewingRecipe = initialRecipe - 1;
			if (currentViewingRecipe < 0)
				currentViewingRecipe = nextCraftingRecipe - 1;
			updateCraftingGraphics();
		}
		else if (backButtonCraftScreen.isTouching(*oldX, *oldY) && backButtonCraftScreen.isColored)
		{
			backButtonCraftScreen.setVisible(false);
			craftButtonCraftScreen.setVisible(false);
			leftButtonCraftScreen.setVisible(false);
			rightButtonCraftScreen.setVisible(false);
			return 1;
		}
		else if (craftButtonCraftScreen.isTouching(*oldX, *oldY) && craftButtonCraftScreen.isColored)
			craftItem();
		leftButtonCraftScreen.setColored(false);
		rightButtonCraftScreen.setColored(false);
		craftButtonCraftScreen.setColored(false);
		backButtonCraftScreen.setColored(false);
	}
	return 0;
}
