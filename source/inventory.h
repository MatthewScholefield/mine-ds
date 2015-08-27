#pragma once
#include <nds.h>
#include <stdio.h>
#include "InvBlock.h"
int getHand();
void setHand(int a);
void changeInvSelectedGraphic(int blockID = -1);
int getInventoryState();
int spaceForItem(int blockID);
bool addInventory(int blockID, int amount, bool direct = false);
bool subInventory(int blockID, int amount);
int checkInventory(int blockID);
int getInventorySlot(int blockID);
void clearInventory(bool direct = false);
bool addInventory(int blockID);
int getBlockAmount(int invSlot);
int getBlockID(int invSlot);
void setBlockID(int slot, int ID);
void setBlockAmount(int slot, int amount);
void saveInventory(FILE* data);
void loadInventory(FILE* data);
void drawInvButtons(bool drawBack, bool survival = true);
void openInventory();
#define NUM_INV_SPACES 30

typedef struct
{
	InvBlock blocks[32];
	//	InvBlock armour[4];
	int hand;
} Inventory;

Inventory &getInventoryRef();