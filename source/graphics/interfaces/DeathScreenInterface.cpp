#include "DeathScreenInterface.h"
#include "interfaceHandler.h"
#include "../../mainGame.h"
#include "../../mobs/mobHandler.h"

#include "../../localizations/locale.h"
#include <string.h>

void DeathScreenInterface::draw()
{
	clearText();
	drawBackground();
	int X = 15-(strlen(locale("you-died"))+1)/2;
	printXY(X, 9, locale("you-died"));
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