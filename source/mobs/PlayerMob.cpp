#include <string>
#include "hurt.h"
#include "mobHandler.h"
#include "mobFunctions.h"
#include "../nifi.h"
#include "../blocks.h"
#include "../world/worldRender.h"
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
Graphic fullHeart(GraphicType::PARTICLE, 0, false);
Graphic halfHeart(GraphicType::PARTICLE, 1, false);
bool PlayerMob::controlsEnabled = true;


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
		}
	}
}

void showHealth(int health)
{
	int i;
	for (i = 0; i < health / 2; ++i)
		fullHeart.draw(25 * 8 - i * 8, 7 * 8);
	if (health % 2 == 1)
		halfHeart.draw(25 * 8 - (health - 1)*4, 7 * 8, true);
}

bool checkLadder(WorldObject &world, int x, int y)
{
	return world.blocks[x / 16][y / 16] == LADDER || world.bgblocks[x / 16][y / 16] == LADDER;
}

void PlayerMob::setControlsEnabled(bool enabled)
{
	controlsEnabled = enabled;
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
				calculateBrightness(world, x / 16, y / 16);
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
			if (world.camCalcX > (WORLD_WIDTH - 1) * 16 - 256) world.camCalcX = (WORLD_WIDTH - 1) * 16 - 256;
			if (world.camCalcY < 0.0) world.camCalcY = 0.0;
			if (world.camCalcY > (WORLD_HEIGHT - 1)*16 - 192) world.camCalcY = (WORLD_HEIGHT - 1)*16 - 192;
			world.camX = int(world.camCalcX);
			world.camY = int(world.camCalcY);

			bool onLadder = false;
			for (int i = -1; i < 2; ++i)
			{
				if (checkLadder(world, x, y + 16 * i))
				{
					onLadder = true;
					break;
				}
			}
			if (controlsEnabled && onLadder && keysHeld() & getGlobalSettings()->getKey(ACTION_CLIMB))
				vy = -3;
			else if (onLadder)
				vy = 2;

			if (controlsEnabled)
			{
				if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_RIGHT) && !collisions[SIDE_RIGHT])
				{
					normalSprite.animate();
					vx = (isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 4.317 : 4.317 * 2;
					facing = false;
					if (getTime() % 10 == 1 && collisions[SIDE_BOTTOM])
						playBlockSfx(world.blocks[int(x / 16)][int((y + 16) / 16)], SOUND_TYPE_STEP, 80);
				}
				else if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_LEFT) && !collisions[SIDE_LEFT])
				{
					normalSprite.animate();
					vx = -1 * ((isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 4.317 : 4.317 * 2);
					facing = true;
					if (getTime() % 10 == 1 && collisions[SIDE_BOTTOM])
						playBlockSfx(world.blocks[int(x / 16)][int((y + 16) / 16)], SOUND_TYPE_STEP, 80);
				}
				else
				{
					normalSprite.setFrame(0);
					vx = 0;
				}

				if (keysDown() & getGlobalSettings()->getKey(ACTION_DROP))
				{
					int blockIDToDrop = getHandID();
					if (subInventory(blockIDToDrop, 1))
						createItemMob(x / 16, y / 16 - 2, blockIDToDrop, 1);
				}
				if ((!onLadder || !checkLadder(world, x, y + 15)) && (canJump(&world) || !isSurvival() || (checkLadder(world, x, y + 16) && !checkLadder(world, x, y + 15))) && !collisions[SIDE_TOP] && (keysHeld() & getGlobalSettings()->getKey(ACTION_JUMP) || keysHeld() & getGlobalSettings()->getKey(ACTION_CLIMB)))
					vy = JUMP_VELOCITY;
			}

			if (y > WORLD_HEIGHTPX) hurt(3, VOID_HURT);
			if (framesHurtSprite == 0) spriteState = 0;
			else --framesHurtSprite;
			showHealth(health);
		}
	}
	if (brightness < 0)
	{
		calcMobBrightness(world);
		mineSprite.paletteID = normalSprite.paletteID;
		mineSprite.setFrame(0);
	}
	if (world.blocks[int(x) / 16][(int(y) + 8) / 16 + 1] != AIR && world.brightness[x / 16][(y + 8) / 16 + 1] != brightness)
	{
		calcMobBrightness(world);
		mineSprite.paletteID = normalSprite.paletteID;
	}
	if (spriteState == 0)
		if (keysHeld() & KEY_TOUCH && canMine() && normalSprite.animFrame == 0)
		{
			if ((keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_LEFT) && !collisions[SIDE_LEFT]) || (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_RIGHT) && !collisions[SIDE_RIGHT]))
				mineSprite.setFrame(0);
			else if (getTime() % 3 == 1)
				mineSprite.animate();
			mineSprite.draw(x - world.camX - 7, (y - world.camY) - 15, facing ? true : false);
		}
		else
			normalSprite.draw(x - world.camX - 7, (y - world.camY) - 15, facing ? true : false);
	else if (spriteState == 1) hurtSprite.draw(x - world.camX - 7, (y - world.camY) - 15, facing ? true : false);
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

void playerMobInit() {
}

