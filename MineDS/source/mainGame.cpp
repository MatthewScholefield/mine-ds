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
#include "mobs/MobPlayer.h"
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
#include <time.h>

WorldObject world;

static void redrawGameUI(void)
{
	lcdMainOnBottom();
	consoleClear();
	drawBackground();
	if (isSurvival())
		drawInvButtons(false);
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
		Button buttons[] = { save, quit, settings };

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

bool isCreative(void)
{
	return (world.gamemode == GAMEMODE_CREATIVE);
}

bool isSurvival(void)
{
	return (world.gamemode == GAMEMODE_SURVIVAL);
}

void newGame(GameMode mode, bool setSeed)
{
	mobsReset();
	if (world.gamemode != GAMEMODE_PREVIEW && setSeed)
		world.seed = getTime();
	srand(world.seed);
	world.gamemode = mode;
	world.CamX = 0;
	world.CamY = 0;
	world.timeInWorld = 0;
	generateWorld(&world);
	if (mode == GAMEMODE_PREVIEW)
	{
		mobHandlerUpdate(&world);
		worldRender_Render(&world, world.CamX, world.CamY);
	}
}

bool loadGame(void)
{
	mobsReset(true);
	if (loadWorld(&world))
		return true;
	return false;
}

void startGame(void)
{
	int oldKeys = keysHeld();
	touchPosition touch;

	consoleClear();
	clear_messages();

	redrawGameUI();
	clearInventory();
	playMusic(MUSIC_HAL2);

	while (!shouldQuitGame())
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
		miningUpdate(&world, world.CamX, world.CamY, touch, keysDown());
		swiWaitForVBlank(); //Should be the only time called in the loop
		worldRender_Render(&world, world.CamX, world.CamY);
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
	mobsReset();
	consoleClear();
	clear_messages();

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
		print_message((char *) buffer);
		world.timeInWorld = 0;
	}
	else
	{
		int i, j;

		lcdMainOnTop();
		for (i = 0; i <= WORLD_WIDTH; ++i)
		{
			for (j = 0; j <= WORLD_HEIGHT; ++j)
			{
				world.blocks[i][j] = BEDROCK;
				world.bgblocks[i][j] = BEDROCK;
			}
		}
		iprintf("Looking for servers\n");
		while (!clientNifiInit()) // Looks for servers, sets up Nifi, and Asks the player to join a server.
			swiWaitForVBlank();
		iprintf("Joining Server!\n");
		// Next Do a HandShake and check that we are communicating with MineDS (and not another game that we might make in the future)
		if (!doHandshake())
			return;
		recieveWorld(&world);
		consoleClear();
		unsigned short buffer[100];
		int client_id = getClientID();
		sprintf((char *) buffer, "%d joined the game.\n", client_id);
		print_message((char *) buffer);
	}
	lcdMainOnBottom();
	world.CamX = 0;
	world.CamY = 0;

	while (!shouldQuitGame())
	{
		scanKeys();
		if (keysDown() & getGlobalSettings()->getKey(ACTION_MENU))
			break;
		recieveWorldUpdate();
		touchRead(&touch);
		nifiUpdate();
		miningUpdate(&world, world.CamX, world.CamY, touch, keysDown());
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
		worldRender_Render(&world, world.CamX, world.CamY);
	}
	nifiDisable();
}

void setSeed(int seed)
{
	world.seed = seed;
}