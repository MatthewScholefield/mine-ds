#include <string>
#include "hurt.h"
#include "mobHandler.h"
#include "mobFunctions.h"
#include "../nifi.h"
#include "../blocks.h"
#include "../worldRender.h"
#include "../graphics/graphics.h"
#include "../inventory.h"
#include "PlayerMob.h"
#include "../general.h"
#include "../blockID.h"
#include "../sounds.h"
#include "../deathScreen.h"
#include "../mainGame.h"
#include "../Config.h"
#include "../mining.h"
#include <time.h>
#include "../graphics/interfaces/interfaceHandler.h"

#define PLAYER_FULL_HEALTH 20
Graphic fullHearts[PLAYER_FULL_HEALTH / 2];
Graphic halfHeart;

void PlayerMob::calcMiscData(WorldObject &world)
{
	calculateMiscData(world, this);
}

void PlayerMob::hurt(int amount, int type)
{
	if (isSurvival() || type == VOID_HURT)
	{
		if (spriteState == 1)
			return;
		if (jumpHurtType(type) && canJump())
			vy = JUMP_VELOCITY;
		playSound(SOUND_PLAYER_HURT);
		health -= amount;
		spriteState = 1;
		framesHurtSprite = 20;
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
			switch (type)
			{
			case CACTUS_HURT:
				message += " was pricked to death";
				break;
			case VOID_HURT:
				message += " fell out of the world";
				break;
			case PLAYER_HURT:
				message += " was killed by a player";
				break;
			case ZOMBIE_HURT:
				message += " was eaten by a zombie";
				break;
			case HEROBRINE_HURT:
				message += " was murdered by a Herobrine";
				break;
			case FALL_HURT:
				message += " hit the ground too hard";
				break;
			default:
				message += " died";
			}
			message += "\n";
			printGlobalMessage((char*) message.c_str());
			if (isSurvival())
				spillInvItems(x, y);
			clearInventory();
			setInterface(INTERFACE_DEATH_SCREEN);
		}
	}
}

void showHealth(int health)
{
	int i;
	for (i = 0; i < health / 2; ++i)
		showGraphic(&fullHearts[i], 256 - 8 * PLAYER_FULL_HEALTH / 2 + i * 8, 56);
	if (health % 2 == 1)
		showGraphic(&halfHeart, 256 - 8 * PLAYER_FULL_HEALTH / 2 + (health - 1)*4, 56);
}

bool checkLadder(WorldObject &world, int x, int y)
{
	return world.blocks[x / 16][y / 16] == LADDER || world.bgblocks[x / 16][y / 16] == LADDER;
}

void PlayerMob::updateMob(WorldObject &world)
{
	if (host)
	{
		if (health > 0)
		{
			if (++tillBrightness > SEC_TO_FPS(1))
			{
				tillBrightness = 0;
				updateBrightnessAround(world, x / 16, y / 16);
			}

			if (getGlobalSettings()->getProperty(PROPERTY_SMOOTH))
			{
				world.camCalcX += (double(x) - 256 / 2 - world.camCalcX)*0.1;
				world.camCalcY += (double(y) - 192 / 2 - world.camCalcY)*0.1;
			}
			else
			{
				world.camCalcX = x - 256 / 2;
				world.camCalcY = y - 192 / 2;
			}

			if (world.camCalcX < 0.0) world.camCalcX = 0.0;
			if (world.camCalcX > WORLD_WIDTH * 16 - 256) world.camCalcX = WORLD_WIDTH * 16 - 256;
			if (world.camCalcY < 0.0) world.camCalcY = 0.0;
			if (world.camCalcY > (WORLD_HEIGHT + 1)*16 - 192) world.camCalcY = (WORLD_HEIGHT + 1)*16 - 192;
			world.camX = int(world.camCalcX);
			world.camY = int(world.camCalcY);

			if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_RIGHT) && !collisions[SIDE_RIGHT])
			{
				animateMob(&normalSprite, 0);
				vx = (isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 4.317 : 4.317 * 2;
				facing = false;
				if (getTime() % 10 == 1 && collisions[SIDE_BOTTOM])
					playBlockSfx(world.blocks[int(x / 16)][int((y + 16) / 16)], SOUND_TYPE_STEP, 80);
			}
			else if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_LEFT) && !collisions[SIDE_LEFT])
			{
				animateMob(&normalSprite, 0);
				vx = -1 * ((isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 4.317 : 4.317 * 2);
				facing = true;
				if (getTime() % 10 == 1 && collisions[SIDE_BOTTOM])
					playBlockSfx(world.blocks[int(x / 16)][int((y + 16) / 16)], SOUND_TYPE_STEP, 80);
			}
			else
			{
				setAnimFrame(&normalSprite, 0, 0);
				vx = 0;
			}

			if (keysDown() & getGlobalSettings()->getKey(ACTION_DROP))
			{
				int blockIDToDrop = getHandID();
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
			if ((!onLadder || !checkLadder(world, x, y + 15)) && (canJump(&world) || !isSurvival() || (checkLadder(world, x, y + 16) && !checkLadder(world, x, y + 15))) && !collisions[SIDE_TOP] && (keysHeld() & getGlobalSettings()->getKey(ACTION_JUMP) || keysHeld() & getGlobalSettings()->getKey(ACTION_CLIMB)))
				vy = JUMP_VELOCITY;

			if (y > WORLD_HEIGHTPX) hurt(3, VOID_HURT);
			if (framesHurtSprite == 0) spriteState = 0;
			else --framesHurtSprite;
			showHealth(health);
		}
	}
	if (brightness < 0)
	{
		loadGraphic(&normalSprite, GRAPHIC_MOB_ANIM, 0, 16, 32, 8 + (6 * (brightness = getBrightness(world, x / 16, (y + 8) / 16 + 1))) / 15); //Walk Animation
		loadGraphic(&hurtSprite, GRAPHIC_MOB, 1, 16, 32, normalSprite.paletteID); //Hurt Graphic
		loadGraphic(&mineSprite, GRAPHIC_MOB_ANIM, 2, 16, 32, normalSprite.paletteID); //Mine Animation
		setAnimFrame(&mineSprite, 0, 1);
	}
	if (world.blocks[int(x) / 16][(int(y) + 8) / 16 + 1] != AIR && getBrightness(world, x / 16, (y + 8) / 16 + 1) != brightness)
		mineSprite.paletteID = hurtSprite.paletteID = normalSprite.paletteID = 8 + (6 * (brightness = getBrightness(world, x / 16, (y + 8) / 16 + 1))) / 15;
	if (spriteState == 0)
		if (keysHeld() & KEY_TOUCH && canMine() && normalSprite.anim_frame == 0)
		{
			if ((keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_LEFT) && !collisions[SIDE_LEFT]) || (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_RIGHT) && !collisions[SIDE_RIGHT]))
				setAnimFrame(&mineSprite, 0, 1);
			else if (getTime() % 3 == 1)
				animateMob(&mineSprite, 0);
			showGraphic(&mineSprite, x - world.camX - 7, (y - world.camY) - 15, facing ? true : false);
		}
		else
			showGraphic(&normalSprite, x - world.camX - 7, (y - world.camY) - 15, facing ? true : false);
	else if (spriteState == 1) showGraphic(&hurtSprite, x - world.camX - 7, (y - world.camY) - 15, facing ? true : false);
}

void PlayerMob::sendWifiUpdate() { }

void PlayerMob::saveToFile(FILE* pFile)
{
	fprintf(pFile, "%d %d %d ", int(x), int(y), health);
}

void PlayerMob::loadFromFile(FILE* pFile)
{
	int loadX, loadY;
	fscanf(pFile, "%d %d %d ", &loadX, &loadY, &health);
	x = loadX;
	y = loadY;
}

bool PlayerMob::isMyPlayer()
{
	return true;
}

bool canPlayerMobSpawnHere(WorldObject &world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world.blocks[x][y + 1]) && isBlockWalkThrough(world.blocks[x][y]) && world.blocks[x][y] != CACTUS && world.blocks[x][y + 1] != CACTUS) return true;
	return false;
}

void playerMobInit()
{
	loadGraphicSub(&fullHearts[0], GRAPHIC_PARTICLE, 0);
	for (int i = 1; i < PLAYER_FULL_HEALTH / 2; ++i)
	{
		//fullHearts[i] = Graphic(fullHearts[0]);
		setCloneGraphic(&fullHearts[0], &fullHearts[i]);
	}
	loadGraphicSub(&halfHeart, GRAPHIC_PARTICLE, 1);
}

