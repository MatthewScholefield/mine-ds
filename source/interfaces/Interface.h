#pragma once
#include <nds.h>
#include "../world.h"

enum InterfaceType
{
	INTERFACE_INVENTORY,
	INTERFACE_CRAFTING,
	INTERFACE_CHEST,
	INTERFACE_FURNACE
};

class Interface
{
protected:

	virtual bool closeOnMovement()
	{
		return true;
	}
public:
	InterfaceType type;
	virtual void update(WorldObject *world, touchPosition *touch) = 0;
	virtual void draw() = 0;

	Interface(InterfaceType type) : type(type) { }

	virtual ~Interface() { }
};