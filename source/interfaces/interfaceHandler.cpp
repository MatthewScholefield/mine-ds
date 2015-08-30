#include "Interface.h"
#include "CraftingInterface.h"
#include "interfaceHandler.h"
#include "InventoryInterface.h"
#include "ChestInterface.h"
#include <memory>

Interface_ptr currentInterface = Interface_ptr(new InventoryInterface(false));

void setInterface(InterfaceType type, int parameter)
{
	switch (type)
	{
	case INTERFACE_INVENTORY:
		currentInterface = Interface_ptr(new InventoryInterface(parameter));
		break;
	case INTERFACE_CRAFTING:
		currentInterface = Interface_ptr(new CraftingInterface());
		break;
	case INTERFACE_CHEST:
		currentInterface = Interface_ptr(new ChestInterface(parameter));
		break;
	default:
		showError("Unknown interface set");
		setInterface(INTERFACE_INVENTORY);
		break;
	}
	currentInterface->draw();
}

void updateInterface(WorldObject *world, touchPosition *touch)
{
	currentInterface->update(world, touch);
	Interface::staticUpdate();
}

void drawInterface()
{
	currentInterface->draw();
}