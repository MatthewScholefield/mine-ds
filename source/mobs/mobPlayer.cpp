#include <string>
#include "hurt.h"
#include "mobHandler.h"
#include "../nifi.h"
#include "../blocks.h"
#include "../worldRender.h"
#include "../graphics/graphics.h"
#include "../inventory.h"
#include "mobPlayer.h"
#include "../general.h"
#include "../blockID.h"
#include "../sounds.h"
#include "../deathScreen.h"
#include "../mainGame.h"
#include "../graphics/inventoryGraphics.h"
#include "../Config.h"
#include "../mining.h"
#include <time.h>

#define PLAYER_FULL_HEALTH 20
#define PLAYER_SPRITE_WALK 0
#define PLAYER_SPRITE_HURT 1
#define PLAYER_SPRITE_MINE 2
int slow = 0;
Graphic playerMobGraphic[3];
Graphic hearts[2];

playerMob::playerMob()
{
	x = 0;
	y = 0;
	vy = 0;
	vx = 0;
	alive = false;
	facing = 0;
	animation = 0;
	mobType = 2;
	health = PLAYER_FULL_HEALTH;
	ping = 0;
	reheal = 0;
	tillBrightness = 0;
	deathScreen = false;
}

playerMob::playerMob(int a, int b)
{
	sx = 6;
	sy = 32;
	x = a;
	y = b;
	vy = 0;
	vx = 0;
	alive = false;
	facing = false;
	mobType = 2;
	health = PLAYER_FULL_HEALTH;
	ping = 0;
	animation = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
	reheal = 0;
	tillBrightness = 0;
	deathScreen = false;
}

void playerMob::hurt(int amount, int type)
{
	if (isSurvival() || type == VOID_HURT)
	{
		if (animation == 1)
			return;
		if (type != VOID_HURT && collisions[0])
			vy = JUMP_VELOCITY;
		playSound(SOUND_PLAYER_HURT);
		health -= amount;
		animation = 1;
		animationClearFrames = 20;
		if (health <= 0)
		{
			std::string message;

			if (isWifi() && isHost() == false)
			{
				unsigned short buffer[10];
				int client_id = getClientID();
				sprintf((char *) buffer, "%d", client_id);
				message = (char*) buffer;
			}
			else if (isWifi())
				message = "The host";
			else
				message = "Steve";
			if (type == CACTUS_HURT) message += " was pricked to death";
			else if (type == VOID_HURT) message += " fell out of the world";
			else if (type == PLAYER_HURT) message += " was killed by a player";
			else if (type == ZOMBIE_HURT) message += " was eaten by a zombie";
			else if (type == HEROBRINE_HURT) message += " was murdered by a Herobrine";
			else message += " died";
			message += "\n";
			printGlobalMessage((char*) message.c_str());
		}
	}
}

void showHealth(int health)
{
	int i;
	for (i = 44; i < (health + 44) - 1; i += 2)
		showGraphic(&hearts[0], i * 4, 56);
	if (health % 2)
		showGraphic(&hearts[1], i * 4, 56);
}

bool checkLadder(WorldObject *world, int x, int y)
{
	return world->blocks[x / 16][y / 16] == LADDER || world->bgblocks[x / 16][y / 16] == LADDER;
}

void playerMob::updateMob(WorldObject* world)
{
	if (host)
	{
		if (health > 0)
		{
			// Every second, update the brightness of visible blocks
			// TODO: This should be in worldRender_Render
			// I would argue not, the player causes the light update, and updating the light levels is not a rendering job ~CoolAs
			if (++tillBrightness > SEC_TO_FPS(1))
			{
				tillBrightness = 0;
				updateBrightnessAround(world, x / 16, y / 16);
			}
			++reheal;
			ping = 0;

			if (getGlobalSettings()->getProperty(PROPERTY_SMOOTH))
			{
				world->camCalcX += (double(x) - 256 / 2 - world->camCalcX)*0.1;
				world->camCalcY += (double(y) - 192 / 2 - world->camCalcY)*0.1;
			}
			else
			{
				world->camCalcX = x - 256 / 2;
				world->camCalcY = y - 192 / 2;
			}

			if (world->camCalcX < 0.0) world->camCalcX = 0.0;
			if (world->camCalcX > WORLD_WIDTH * 16 - 256) world->camCalcX = WORLD_WIDTH * 16 - 256;
			if (world->camCalcY < 0.0) world->camCalcY = 0.0;
			if (world->camCalcY > (WORLD_HEIGHT + 1)*16 - 192) world->camCalcY = (WORLD_HEIGHT + 1)*16 - 192;
			world->camX = int(world->camCalcX);
			world->camY = int(world->camCalcY);

			if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_RIGHT) && !collisions[1])
			{
				closeChest(); //Close a chest, if open
				animateMob(&playerMobGraphic[PLAYER_SPRITE_WALK], 0);
				vx = (isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 4.317 : 4.317 * 2;
				facing = false;
			}
			else if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_LEFT) && !collisions[2])
			{
				closeChest();
				animateMob(&playerMobGraphic[PLAYER_SPRITE_WALK], 0);
				vx = -1 * ((isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 4.317 : 4.317 * 2);
				facing = true;
			}
			else
			{
				setAnimFrame(&playerMobGraphic[PLAYER_SPRITE_WALK], 0, 0);
				vx = 0;
			}

			if (keysDown() & getGlobalSettings()->getKey(ACTION_DROP))
			{
				int blockIDToDrop = getBlockID(getSelectedSlot());
				if (subInventory(blockIDToDrop, 1))
					createItemMob(x / 16, y / 16 - 2, blockIDToDrop, 1);
			}
			bool onLadder = false;
			for (int i = -1; i < 2; ++i)
			{
				if (checkLadder(world, x, y + 16 * i))
				{
					onLadder = true;
					break;
				}
			}
			if (onLadder && keysHeld() & getGlobalSettings()->getKey(ACTION_CLIMB))
				vy = -3;
			else if (onLadder)
				vy = 2;
			if ((!onLadder || !checkLadder(world, x, y + 15)) && (collisions[0] || !isSurvival() || (checkLadder(world, x, y + 16) && !checkLadder(world, x, y + 15))) && !collisions[3] && (keysHeld() & getGlobalSettings()->getKey(ACTION_JUMP) || keysHeld() & getGlobalSettings()->getKey(ACTION_CLIMB)))
				vy = JUMP_VELOCITY;

			if (y > WORLD_HEIGHTPX) hurt(3, VOID_HURT);
			if (animationClearFrames == 0) animation = 0;
			else --animationClearFrames;
			if (reheal > 300)
			{
				if (health < 20)
					++health;
				reheal = 0;
			}
			showHealth(health);
		}
		else if (deathScreen == false)
		{
			clearInventory();
			deathScreenSetup();
			deathScreen = true;
			health = -50;
		}
		else
		{
			x = 1024;
			y = 1024;
			int result = deathScreenUpdate();
			if (result == 0)
			{
				int i, j;
				for (i = 0; i <= WORLD_WIDTH; ++i)
					for (j = 0; j <= WORLD_HEIGHT; ++j)
					{
						if (canPlayerMobSpawnHere(world, i, j))
						{
							x = i * 16;
							y = j * 16 - 16;
							deathScreen = false;
							health = PLAYER_FULL_HEALTH;
							i = WORLD_WIDTH + 1;
							j = WORLD_HEIGHT + 1;
							animation = 0;
						}
					}
			}
			else if (result == 1)
				quitGame();
		}
	}
	if (x - world->camX>-16 && x - world->camX < 256 + 16 && y - world->camY>-32 && y - world->camY < 256)
	{
		if (animation == 0)
			if (keysHeld() & KEY_TOUCH && canMine() && playerMobGraphic[PLAYER_SPRITE_WALK].anim_frame == 0)
			{
				if ((keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_LEFT) && !collisions[2]) || (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_RIGHT) && !collisions[1]))
					setAnimFrame(&playerMobGraphic[PLAYER_SPRITE_MINE], 0, 1);
				else if (getTime() % 3 == 1)
					animateMob(&playerMobGraphic[PLAYER_SPRITE_MINE], 0);
				showGraphic(&playerMobGraphic[PLAYER_SPRITE_MINE], x - world->camX - 7, (y - world->camY) - 15, facing ? true : false);
			}
			else
				showGraphic(&playerMobGraphic[PLAYER_SPRITE_WALK], x - world->camX - 7, (y - world->camY) - 15, facing ? true : false);
		else if (animation == 1) showGraphic(&playerMobGraphic[PLAYER_SPRITE_HURT], x - world->camX - 7, (y - world->camY) - 15, facing ? true : false);
	}
}

void playerMob::sendWifiUpdate()
{
}

void playerMob::saveToFile(FILE* pFile)
{
	fprintf(pFile, "%d %d %d ", int(x), int(y), health);
}

void playerMob::loadFromFile(FILE* pFile)
{
	int loadX, loadY;
	fscanf(pFile, "%d %d %d ", &loadX, &loadY, &health);
	x = loadX;
	y = loadY;
}

bool playerMob::isMyPlayer()
{
	return true;
}

bool canPlayerMobSpawnHere(WorldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS && world->blocks[x][y + 1] != CACTUS) return true;
	return false;
}

void playerMobInit()
{
	loadGraphicAnim(&playerMobGraphic[PLAYER_SPRITE_WALK], 0); //Walk Animation
	loadGraphic(&playerMobGraphic[PLAYER_SPRITE_HURT], true, 1); //Hurt Graphic
	loadGraphicAnim(&playerMobGraphic[PLAYER_SPRITE_MINE], 2); //Mine Animation
	loadGraphicSub(&hearts[0], 0, 0);
	loadGraphicSub(&hearts[1], 0, 1);
}

