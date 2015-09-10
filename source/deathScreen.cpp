#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "mining.h"
#include <stdio.h>
#include <nds.h>
#include "sounds.h"
#include "graphics/UI.h"
#include "general.h"
#include "inventory.h"
#include "mainGame.h"
bool showingDeathScreen = false;

Button respawnButton(8, 10, "Respawn", 14);
Button deathToTitleScreenButton(8, 15, "TitleScreen", 14);

void setupDeathScreen()
{
	oamClear(&oamSub, 0, 0); //Clear sprites
	showingDeathScreen = true;
	clearText();
	lcdMainOnTop();
	drawBackground();
	iprintf("\x1b[8;11HYou Died!");
	setMiningDisabled(true);
	respawnButton.draw();
	deathToTitleScreenButton.draw();
}

int deathScreenUpdate(touchPosition *touch)
{
	if (!showingDeathScreen)
		return 2;
	int returnVal = 2;
	if (keysDown() & KEY_TOUCH)
	{
		touchRead(touch);
		respawnButton.setColored(respawnButton.isTouching(touch->px, touch->py));
		deathToTitleScreenButton.setColored(deathToTitleScreenButton.isTouching(touch->px, touch->py));
	}
	else if (keysUp() & KEY_TOUCH)
	{
		if (respawnButton.isTouching(touch->px, touch->py) && respawnButton.isColored)
		{
			showingDeathScreen = false;
			drawBackground();
			clearText();
			lcdMainOnBottom();
			setMiningDisabled(false);
			returnVal = 0;
		}
		else if (deathToTitleScreenButton.isTouching(touch->px, touch->py) && deathToTitleScreenButton.isColored)
		{
			showingDeathScreen = false;
			setMiningDisabled(false);
			returnVal = 1;
		}
		respawnButton.setColored(false);
		deathToTitleScreenButton.setColored(false);
	}
	return returnVal;
}
