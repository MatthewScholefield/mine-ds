#include <nds.h>
#include "Interface.h"

#pragma once

void drawInterface();
void setInterface(WorldObject &world, InterfaceType type, int parameter = 0);
void updateInterface(WorldObject &world, touchPosition &touch);