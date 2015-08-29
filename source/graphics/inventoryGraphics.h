#pragma once
#include "../world.h"
#define INV_SLOTS 20;
void drawSlots(int selectedSlot, int startX, int startY, int xCount = 15, int yCount = 2, int xSpace = 2, int ySpace = 3);
void drawInvGraphics();
void drawSelectedFrame();
void enableInvGraphics();
void updateChestItems();
int getChestBlockID(int slot);