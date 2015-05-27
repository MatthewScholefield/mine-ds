#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "graphics/inventoryGraphics.h"
#include "mobs/mobHandler.h"
#include "mining.h"
#include <stdio.h>
#include <nds.h>
#include "sounds.h"
#include "titlescreen.h"
#include "general.h"
bool mustPrintDeathScreen = true;
bool died = false; //Used to show whether exitted to titlescreen after dying
touchPosition *touch;
touchPosition *oldTouch;
uint32 oldKeys;

Button *respawnButton;
Button *deathToTitleScreenButton;

/*bool getDied()
{
	return died;
}*/

void deathScreenSetup()
{
	if (mustPrintDeathScreen == true)
	{
		disableInvGraphics();
		consoleClear();
		lcdMainOnTop();
		drawBackground();
		printf("\x1b[8;11HYou Died!");
		drawButton(8, 10, 14);
		drawButton(8, 15, 14);
		//printXY(12, 11, "Respawn");
		//printXY(10, 16, "TitleScreen");
		//stopMusic();
		mustPrintDeathScreen = false;
		oldKeys = keysHeld();
		setMiningDisabled(true);
		respawnButton = new Button(8, 10, "Respawn", 14);
		deathToTitleScreenButton = new Button(8, 15, "TitleScreen", 14);
		touch = new touchPosition;
	}
}

int deathScreenUpdate()
{
	if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
	{
		touchRead(touch);
		respawnButton->setColored(respawnButton->isTouching(touch->px, touch->py));
		deathToTitleScreenButton->setColored(deathToTitleScreenButton->isTouching(touch->px, touch->py));
	}
	else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
	{
		if (respawnButton->isTouching(touch->px, touch->py) && respawnButton->isColored)
		{
			drawBackground();
			mustPrintDeathScreen = true;
			consoleClear();
			enableInvGraphics();
			lcdMainOnBottom();
			setMiningDisabled(false);
			died = false;
			return 0;
		}
		else if (deathToTitleScreenButton->isTouching(touch->px, touch->py) && deathToTitleScreenButton->isColored)
		{
			drawBackground();
			mustPrintDeathScreen = true;
			setMiningDisabled(false);
			died = true;
			return 1;
		}
		respawnButton->setColored(false);
		deathToTitleScreenButton->setColored(false);
	}
	oldKeys = keysHeld();
	touchRead(touch);
	return 2;
}
