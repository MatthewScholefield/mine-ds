#include "Interface.h"
#include "CraftingInterface.h"
#include "interfaceHandler.h"
#include "InventoryInterface.h"
#include <memory>

Interface_ptr currentInterface = Interface_ptr(new InventoryInterface());

void setInterface(InterfaceType type)
{
	switch (type)
	{
	case INTERFACE_INVENTORY:
		currentInterface = Interface_ptr(new InventoryInterface());
		break;
	case INTERFACE_CRAFTING:
		currentInterface = Interface_ptr(new CraftingInterface());
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
}