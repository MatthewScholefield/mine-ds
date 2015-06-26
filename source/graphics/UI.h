#pragma once
#include <string>
#include "Button.h"

void startTransition(bool forward);
int createMenu(Button buttons[], int size, bool enableBack = true, int scrollLength = -1);
bool enableDisableMenu(bool initial);
int listMenu(int x, int y, int numItems, int maxNameLength);
bool createDialog(std::string message, bool cancel = true, bool firstHalf = true);
void drawBackground(bool firstSlot = true);