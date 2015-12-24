#pragma once
#include <nds.h>
#include <memory>
#include "../../WorldObject.h"

enum InterfaceType
{
	INTERFACE_INVENTORY,
	INTERFACE_CRAFTING,
	INTERFACE_CHEST,
	INTERFACE_FURNACE,
	INTERFACE_PAGE,
	INTERFACE_DEATH_SCREEN
};

class Interface
{
protected:
	static bool shouldUpdate;
	virtual bool closeOnMovement()
	{
		return true;
	}
public:
	InterfaceType type;
	virtual void update(WorldObject &world, touchPosition &touch) = 0;
	virtual void draw() = 0;

	static void triggerUpdate()
	{
		shouldUpdate = true;
	}

	static void staticUpdate()
	{
		shouldUpdate = false;
	}


	Interface(InterfaceType type) : type(type) { }

	virtual ~Interface() { }
};

using Interface_ptr = std::shared_ptr<Interface>;