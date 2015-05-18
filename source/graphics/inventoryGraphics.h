#pragma once
#include "../world.h"
#define INV_SLOTS 20;
void initInvGraphics();
void updateInvGraphics();
//bool invFull();
void drawInv();
void disableInvGraphics();
void enableInvGraphics();
void openChest(WorldObject *world, int x, int y, bool bg);
void closeChest();
void drawChestItems();
void updateChestItems();
int getOpenedChestID();
int getChestBlockID(int slot);