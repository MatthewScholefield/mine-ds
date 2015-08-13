#include "Interface.h"

#pragma once

class InventoryInterface : public Interface
{
public:

	void update(WorldObject *world, touchPosition *touch) { }

	void draw() { }

	InventoryInterface() : Interface(INTERFACE_INVENTORY) { }

	~InventoryInterface() { }
};