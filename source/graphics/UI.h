#pragma once
#include <string>
#include "Button.h"

void startTransition(bool forward);
int createMenu(Button buttons[], int size, bool enableBack = true, int scrollLength = -1);
bool enableDisableMenu(bool initial);
int listMenu(int x, int y, int numItems, int maxNameLength);
bool createDialog(std::string message, bool cancel = true, bool firstHalf = true, bool mineDS = true);
void drawBackground(bool firstSlot = true, bool mineDS = true);
void drawInvSlot(int x, int y, int tile = 154);
bool touchesTileBox(const touchPosition &touch, int x, int y, int sx, int sy);