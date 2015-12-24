#include "DeathScreenInterface.h"
#include "interfaceHandler.h"
#include "../../mainGame.h"
#include "../../mobs/mobHandler.h"

void DeathScreenInterface::draw()
{
	clearText();
	drawBackground();
	printXY(11, 9, "You Died!");
	menu.draw();
}

void DeathScreenInterface::update(WorldObject& world, touchPosition& touch)
{
	switch (menu.update(touch))
	{
	case RESPAWN:
		triggerPlayerRespawn();
		setInterface(world, INTERFACE_INVENTORY);
		break;
	case TITLESCREEN:
		quitGame();
		break;
	}
}