#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "mining.h"
#include <stdio.h>
#include <nds.h>
#include "sounds.h"
#include "titlescreen.h"
#include "general.h"
bool deathScreenShow = true;
touchPosition touch;
int oldKeys;
bool died = false; //Used to show whether exitted to titlescreen after dying

/*bool getDied()
{
	return died;
}*/

void deathScreenSetup()
{
	if (deathScreenShow == true)
	{
		consoleClear();
		lcdMainOnTop();
		drawBackground();
		printf("\x1b[8;10HYou Died!");
		drawButton(8, 10, 14);
		drawButton(8, 15, 14);
		printXY(12, 11, "Respawn");
		printXY(10, 16, "TitleScreen");
		stopMusic();
		deathScreenShow = false;
		oldKeys = keysHeld();
		touchRead(&touch);
		miningSetScene(true);
	}
}

int deathScreenUpdate()
{
	if (deathScreenShow == false)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			if (touch.px > 64 && touch.px < 184 && touch.py > 80 && touch.py < 104)
				drawButtonColored(8, 10, 14);
			else if (touch.px > 64 && touch.px < 184 && touch.py > 120 && touch.py < 152)
				drawButtonColored(8, 15, 14);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 64 && touch.px < 184 && touch.py > 80 && touch.py < 104)
			{
				drawBackground();
				deathScreenShow = true;
				consoleClear();
				printf("\n\n\n\n\n\n\n\n");
				lcdMainOnBottom();
				miningSetScene(false);
				died = false;
				return false;
			}
			else drawButton(8, 10, 14);
			if (touch.px > 64 && touch.px < 184 && touch.py > 120 && touch.py < 144)
			{
				drawBackground();
				deathScreenShow = true;
				miningSetScene(false);
				died = true;
				return 1;
			}
			else drawButton(8, 15, 14);
		}
		oldKeys = keysHeld();
		touchRead(&touch);
	}
	return 2;
}
