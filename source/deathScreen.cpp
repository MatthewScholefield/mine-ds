#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "graphics/inventoryGraphics.h"
#include "mobs/mobHandler.h"
#include "mining.h"
#include <stdio.h>
#include <nds.h>
#include "sounds.h"
#include "graphics/UI.h"
#include "general.h"
#include "inventory.h"
#include "mainGame.h"
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
		oamClear(&oamSub, 0, 0);
		oamUpdate(&oamSub);
		disableInvGraphics();
		clearText();
		lcdMainOnTop();
		drawBackground();
		printf("\x1b[8;11HYou Died!");
		mustPrintDeathScreen = false;
		oldKeys = keysHeld();
		setMiningDisabled(true);
		delete respawnButton;
		delete deathToTitleScreenButton;
		delete touch;
		respawnButton = new Button(8, 10, "Respawn", 14);
		deathToTitleScreenButton = new Button(8, 15, "TitleScreen", 14);
		touch = new touchPosition;
		respawnButton->draw();
		deathToTitleScreenButton->draw();
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
			clearText();
			enableInvGraphics();
			lcdMainOnBottom();
			setMiningDisabled(false);
			drawInvButtons(false, isSurvival());
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
