#pragma once
#include <nds.h>
#include <stdio.h>
#include "world.h"
int getInventoryState();
int spaceForItem(int blockID);
void addInventory(int blockID, int amount, bool direct = false);
bool subInventory(int blockID, int amount);
int checkInventory(int blockID);
int checkInventorySlot(int slot);
int getInventorySlot(int blockID);
void clearInventory(bool direct = false);
void addInventory(int blockID);
int getBlockAmount(int invSlot);
int getBlockID(int invSlot);
void saveInventory(FILE* data);
void loadInventory(FILE* data);
void updateInventory(touchPosition touch, WorldObject* world, uint oldKeys);
void drawInvButtons(bool drawBack, bool survival = true);
void openInventory();
#define NUM_INV_SPACES 30

typedef struct {
	int blockId = 0;
	int blockAmount = 0;
} InvBlock;

typedef struct {
	InvBlock blocks[32];
	//	InvBlock armour[4];
	int hand;
} Inventory;
