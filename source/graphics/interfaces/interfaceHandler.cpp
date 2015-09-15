#include "Interface.h"
#include "CraftingInterface.h"
#include "interfaceHandler.h"
#include "InventoryInterface.h"
#include "ChestInterface.h"
#include "PageInterface.h"
#include "FurnaceInterface.h"
#include "DeathScreenInterface.h"
#include <memory>

Interface_ptr currentInterface = nullptr;

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
	case INTERFACE_FURNACE:
		currentInterface = Interface_ptr(new FurnaceInterface());
		break;
	case INTERFACE_PAGE:
		currentInterface = Interface_ptr(new PageInterface());
		break;
	case INTERFACE_DEATH_SCREEN:
		currentInterface = Interface_ptr(new DeathScreenInterface());
		break;
	default:
		showError("Unknown interface set");
		setInterface(INTERFACE_INVENTORY);
		break;
	}
	currentInterface->draw();
}

void updateInterface(WorldObject &world, touchPosition &touch)
{
	if (!currentInterface)
		setInterface(INTERFACE_INVENTORY, false);
	currentInterface->update(world, touch);
	Interface::staticUpdate();
}

void drawInterface()
{
	currentInterface->draw();
}