#pragma once
#include <string>
#include "Button.h"

int menu(Button buttons[], int size, bool enableBack = true);
bool createDialog(std::string message, bool cancel = true);
void drawBackground();