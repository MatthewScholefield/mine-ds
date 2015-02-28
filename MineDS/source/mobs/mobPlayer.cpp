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
	onground = false;
	facing = 0;
	animation = 0;
	mobtype = 2;
	health = PLAYER_FULL_HEALTH;
	ping = 0;
	reheal = 0;
	tillBrightness = 0;
	deathscreen = false;
}

playerMob::playerMob(int a, int b)
{
	gravity = 3;
	gravityValue = 3;
	sx = 6;
	sy = 32;
	x = a;
	y = b;
	vy = 0;
	vx = 0;
	alive = false;
	onground = false;
	facing = false;
	mobtype = 2;
	health = PLAYER_FULL_HEALTH;
	ping = 0;
	animation = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
	reheal = 0;
	tillBrightness = 0;
	deathscreen = false;
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
		animationclearframes = 20;
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

void playerMob::updateMob(worldObject* world)
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

			world->CamCalcX += (double(x) - 256 / 2 - world->CamCalcX)*0.1;
			world->CamCalcY += (double(y) - 192 / 2 - world->CamCalcY)*0.1;

			if (world->CamCalcX < 0.0) world->CamCalcX = 0.0;
			world->CamX =int(world->CamCalcX);
			if (world->CamCalcY < 0.0) world->CamCalcY = 0.0;
			world->CamY = int(world->CamCalcY);
			
			if (world->CamX > WORLD_WIDTH * 16 - 256) world->CamX = WORLD_WIDTH * 16 - 256;
			if (world->CamY > (WORLD_HEIGHT + 1)*16 - 192) world->CamY = (WORLD_HEIGHT + 1)*16 - 192;
			if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_RIGHT) && !collisions[1] && !collisions[3])
			{
				animateMob(&playerMobGraphic[0], 0);
				x += (isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 1 : 2;
				facing = false;
			}
			else if (keysHeld() & getGlobalSettings()->getKey(ACTION_MOVE_LEFT) && !collisions[2] && !collisions[3])
			{
				animateMob(&playerMobGraphic[0], 0);
				x -= (isSurvival() || !getGlobalSettings()->getProperty(PROPERTY_SPEED)) ? 1 : 2;
				facing = true;
			}
			else
				setAnimFrame(&playerMobGraphic[0], 0, 0);

			if ((collisions[0] || !isSurvival()) && (keysHeld() & getGlobalSettings()->getKey(ACTION_JUMP) || keysHeld() & getGlobalSettings()->getKey(ACTION_CLIMB)))
				vy = JUMP_VELOCITY;

			if (y > WORLD_HEIGHTPX) hurt(3, VOID_HURT);
			if (animationclearframes == 0) animation = 0;
			else --animationclearframes;
			if (reheal > 300)
			{
				if (health < 20)
					++health;
				reheal = 0;
			}
			showHealth(health);
		}
		else if (deathscreen == false)
		{
			clearInventory();
			deathScreenSetup();
			deathscreen = true;
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
							deathscreen = false;
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
	if (x - world->CamX>-16 && x - world->CamX < 256 + 16 && y - world->CamY>-32 && y - world->CamY < 256)
	{
		if (animation == 0)
			if (keysHeld() & KEY_TOUCH)
			{
				if (getTime() % 3 == 1)
					animateMob(&playerMobGraphic[PLAYER_SPRITE_MINE], 0);
				showGraphic(&playerMobGraphic[PLAYER_SPRITE_MINE], x - world->CamX - 5, y - world->CamY, facing ? true : false);
			}
			else
				showGraphic(&playerMobGraphic[PLAYER_SPRITE_WALK], x - world->CamX - 5, y - world->CamY, facing ? true : false);
		else if (animation == 1) showGraphic(&playerMobGraphic[PLAYER_SPRITE_HURT], x - world->CamX - 5, y - world->CamY, facing ? true : false);
	}
}

void playerMob::sendWifiUpdate()
{
}

void playerMob::saveToFile(FILE* pFile)
{
	fprintf(pFile, "%d %d %d ", x, y, health);
}

void playerMob::loadFromFile(FILE* pFile)
{
	fscanf(pFile, "%d %d %d ", &x, &y, &health);
}

bool playerMob::isMyPlayer()
{
	return true;
}

bool canPlayerMobSpawnHere(worldObject* world, int x, int y)
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

