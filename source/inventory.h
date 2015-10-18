#pragma once
#include <stdio.h>
#include "InvBlock.h"

#define NUM_INV_SPACES 30
#define MAX_PER_STACK 64

class Inventory
{
public:
	InvBlock blocks[32];
	int hand;

	Inventory() : blocks { }
, hand(-1) { }
};

Inventory &getInventoryRef();
bool addInventory(int blockID, int amount = 1);
bool subInventory(int blockID, int amount);
int checkInventory(int blockID);
void clearInventory();
int getHandID();
void saveInventory(FILE* data);
void loadInventory(FILE* data);
void spillInvItems(int x, int y);