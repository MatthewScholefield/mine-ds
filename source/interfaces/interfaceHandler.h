#include <nds.h>
#include "Interface.h"

#pragma once

void drawInterface();
void setInterface(InterfaceType type, bool open = false);
void updateInterface(WorldObject *world, touchPosition *touch);