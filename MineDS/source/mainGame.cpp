#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldgen.h"
#include "general.h"
#include "mining.h"
#include "blockID.h"
#include "inventory.h"
#include "worldRender.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "mobs/mobPlayer.h"
#include "deathScreen.h"
#include "daynight.h"
#include "graphics/inventoryGraphics.h"
#include "inventory.h"
#include "titlescreen.h"
#include "Config.h"
#include "files.h"
#include "mainGame.h"
#include "sounds.h"
#include "nifi.h"
#include "communications.h"
#include "blockupdate.h"
#include "blockPages.h"
#include <time.h>

bool shouldQuitGame = false;
worldObject world;

static void redrawGameUI(void)
{
	lcdMainOnBottom();
	consoleClear();
	drawBackground();
	drawInvButtons(false, isSurvival());
	if (!isSurvival())
		printXY(1, 15, getPageName(getBlockPage()));
	updateInvGraphics();
}

static int inGameMenu()
{
	bool backbutton = false;

	while (!backbutton)
	{
		lcdMainOnTop();
		drawBackground();
		consoleClear();

		Button save = Button(10, 8, "Save Game", 11);
		Button quit = Button(10, 13, "Quit Game", 11);
		Button settings = Button(10, 18, "Settings", 11);
		Button buttons[] = {save, quit, settings};

		switch (menu(buttons, 3))
		{
			case 1: // save game
				printXY(1, 22, "Saving game");
				if (!saveWorld(&world))
				{
					printXY(1, 22, "Failed to save game");
					sleep(1);
				}
				break;
			case 2: // quit game
				return -1;
			case 3: // settings
				settingsScreen();
				break;
			default: // back button
				backbutton = true;
				break;
		}
	}
	redrawGameUI();
	return 0;
}

/*bool isCreative(void)
{
	return (world.gamemode == GAMEMODE_CREATIVE);
}*/

bool isSurvival(void)
{
	return (world.gamemode == GAMEMODE_SURVIVAL);
}

void quitGame()
{
	shouldQuitGame = true;
}

void newGame(gamemode_t mode, int seed)
{
	// Zero for a random seed
	if (seed == 0)
		world.seed = time(NULL);
	srand(world.seed);
	mobsReset();
	clearInventory();
	shouldQuitGame = false;
	world.gamemode = mode;
	world.camX = 0;
	world.camY = 0;
	world.camCalcX = 0.0;
	world.camCalcY = 0.0;
	world.timeInWorld = 0;
	generateWorld(&world);
}

void previewGame(void)
{
	newGame(GAMEMODE_PREVIEW, 0);
	mobHandlerUpdate(&world);
	if (getGlobalSettings()->getProperty(PROPERTY_SMOOTH))
		worldRender_Render(&world, world.camX, int(10 * world.camCalcY));
	else
		worldRender_Render(&world, world.camX, int(world.camCalcY));
}

bool loadGame(void)
{
	mobsReset(true);
	shouldQuitGame = false;
	if (loadWorld(&world))
	{
		srand(world.seed);
		return true;
	}
	return false;
}

void joinGame(void)
{
	fillWorld(&world, BEDROCK);
	iprintf("Looking for servers\n");
	// TODO: Rename clientNifiInit() to something that makes more sense
	while (!clientNifiInit()) // Looks for servers, sets up Nifi, and Asks the player to join a server.
		swiWaitForVBlank();
	iprintf("Joining Server!\n");
	if (!doHandshake())
		return;
	recieveWorld(&world);
	// TODO: Move this to nifi.cpp
	unsigned short buffer[100];
	int client_id = getClientID();
	sprintf((char *) buffer, "%d joined the game.\n", client_id);
	printGlobalMessage((char *) buffer);
	startMultiplayerGame(false);
}

void startGame(void)
{
	int oldKeys = keysHeld();
	touchPosition touch;

	consoleClear();
	clear_messages();

	redrawGameUI();
	playMusic(MUSIC_HAL2);
	if (!isSurvival())
		setBlockPage(PAGE_WOOL);

	while (!shouldQuitGame)
	{
		updateTime();
		scanKeys();
		mobHandlerUpdate(&world);
		updateInventory(touch, &world, oldKeys);
		update_message();
		if (keysHeld() & KEY_B && keysHeld() & KEY_DOWN)
			clear_messages();
		if (keysDown() & getGlobalSettings()->getKey(ACTION_MENU) && getInventoryState() == 0)
		{
			if (inGameMenu() != 0)
				break;
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		miningUpdate(&world, world.camX, world.camY, touch, keysDown());
		proceduralBlockUpdate(&world);
		swiWaitForVBlank(); //Should be the only time called in the loop
		worldRender_Render(&world, world.camX, world.camY);
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
		timeUpdate(&world);
	}
}

// TODO: clean this function up

void startMultiplayerGame(bool host)
{
	touchPosition touch;

	nifiEnable();
	consoleClear();
	clear_messages();

	drawBackground();
	playMusic(MUSIC_HAL2);
	mobsReset();

	if (host)
	{
		lcdMainOnBottom();
		iprintf("Generating World!\n");
		generateWorld(&world);
		while (!hostNifiInit()) swiWaitForVBlank();
		communicationInit(&world);
		consoleClear();
		unsigned short buffer[100];
		int server_id = getServerID();
		sprintf((char *) buffer, "Server ID: %d\n", server_id);
		printGlobalMessage((char *) buffer);
		world.timeInWorld = 0;
	}
	lcdMainOnBottom();
	world.timeInWorld = 0;
	world.camX = 0;
	world.camY = 0;

	while (!shouldQuitGame)
	{
		scanKeys();
		if (keysDown() & getGlobalSettings()->getKey(ACTION_MENU))
			break;
		recieveWorldUpdate();
		touchRead(&touch);
		nifiUpdate();
		miningUpdate(&world, world.camX, world.camY, touch, keysDown());
		update_message();
		mobHandlerUpdate(&world);
		if (keysDown() & getGlobalSettings()->getKey(ACTION_MENU))
			break;
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
		if (host)
			timeUpdate(&world);
		worldRender_Render(&world, world.camX, world.camY);
	}
	nifiDisable();
}

/*void setSeed(int seed)
{
	world.seed = seed;
}*/
