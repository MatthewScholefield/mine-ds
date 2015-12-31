#include <nds.h>
#include "Interface.h"

#pragma once

void drawInterface();
void setInterface(World &world, InterfaceType type, int parameter = 0);
void updateInterface(World &world, touchPosition &touch);