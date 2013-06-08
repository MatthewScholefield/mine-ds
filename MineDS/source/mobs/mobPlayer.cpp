#include <stdio.h>
#include <string>
#include "hurt.h"
#include "../nifi.h"
#include "../world.h"
#include "../blocks.h"
#include "../worldRender.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "mobPlayer.h"
#include "../message.h"
#include "../blockID.h"
#include "../sounds.h"
#include <nds.h>
#include "mobHandler.h"
#include "../deathScreen.h"
#include"../titlescreen.h"
//ASDF?
#define PLAYER_FULL_HEALTH 20
bool quitGame = false;
int slow=0;
Graphic playerMobGraphic[3];
Graphic hearts[2];
bool shouldQuitGame()
{
	return quitGame;
}
playerMob::playerMob()
{
	x=0;
	y=0;
	vy=0;
	vx=0;
	alive=false;
	onground=false;
	facing=0;
	animation=0;
	mobtype=2;
	health=PLAYER_FULL_HEALTH;
	ping=0;
	reheal=0;
	tillBrightness=0;
	deathscreen=false;
	quitGame=false;
}
playerMob::playerMob(int a,int b)
{
	gravity=3;
	gravityValue=3;
	sx=6;
	sy=32;
	x=a;
	y=b;
	vy=0;
	vx=0;
	alive=false;
	onground=false;
	facing=false;
	mobtype=2;
	health=PLAYER_FULL_HEALTH;
	ping=0;
	animation=0;
	timeTillWifiUpdate=rand()%4+4;
	reheal=0;
	tillBrightness=0;
	deathscreen=false;
	quitGame=false;
}
void playerMob::hurt(int amount,int type)
{
	if (isSurvival() || type==VOID_HURT)
	{
		if (animation==1)
			return;
		if (type!=VOID_HURT)
			vy-=2;
		playSound(PLAYER_H);
		y+=vy;
		health-=amount;
		animation=1;
		animationclearframes=20;
		if (health<=0)
		{
			std::string message;

			if (isWifi() && isHost()==false)
			{
				unsigned short buffer[10];
				int client_id = getClientID();	
				sprintf((char *)buffer,"%d", client_id);
				message = (char*)buffer;
			}		
			else if (isWifi())
				message = "The host";
			else
				message = "Steve";
			if (type==CACTUS_HURT) message += " was pricked to death";
			else if (type==VOID_HURT) message += " fell out of the world";
			else if (type==PLAYER_HURT) message += " was killed by a player";	
			else if (type==ZOMBIE_HURT) message += " was eaten by a zombie";		
			else message += " died";
			message+="\n";
			print_message((char*)message.c_str());
		}
	}
}
void showHealth(int health)
{
	int i;
	for (i=0;i<health-1;i+=2)
	{
		showGraphic(&hearts[0],i*4,56);
	}
	if (health%2)
		showGraphic(&hearts[1],i*4,56);
}
void playerMob::updateMob(worldObject* world)
{
	if (host)
	{
		if (health>0)
		{
			tillBrightness++;
			if (tillBrightness>60)
			{
				tillBrightness=0;
				updateBrightnessAround(world,x/16,y/16);
			}
			reheal++;
			ping=0;
			world->CamX=x-256/2;
			world->CamY=y-192/2;
			if( world->CamX <0) world->CamX = 0;
			if (world->CamY<0) world->CamY = 0;
			if( world->CamX>WORLD_WIDTH*16-256) world->CamX = WORLD_WIDTH*16-256;
			if (world->CamY>(WORLD_HEIGHT+1)*16-192) world->CamY = (WORLD_HEIGHT+1)*16-192;
			if (keysHeld()&KEY_RIGHT && !collisions[1] && !collisions[3]){ x++; facing=false;}
			if (keysHeld()&KEY_LEFT && !collisions[2] && !collisions[3])
			{ x--; facing=true; }
			if (collisions[3]==true && world->blocks[x/16][y/16] != LADDER)
			{
				vy=0;
				y+=1;
			}
			if (collisions[0]==false && (world->blocks[x/16][y/16] != LADDER || (keysHeld() & KEY_DOWN)))
			{
				y+=vy; 
			}
			/*else if ((keysHeld() & KEY_DOWN) && (world->blocks[x/16][y/16] == LADDER || (keysHeld() & KEY_DOWN)) && collisions[0]==false)
				y++;***/
		/*else if (!(keysHeld() & KEY_UP) && (world->blocks[x/16][y/16] == LADDER || world->blocks[x/16][(y/16)-1] == LADDER || world->blocks[x/16][(y/16)-2] == LADDER) && collisions[0]==false)
			{
				slow ++;
				if (slow == 5)
					{
						y ++;
						slow = 0;
				}
			}*/
			else vy=0;
			if ((keysDown() & KEY_UP || keysDown() & KEY_A) && collisions[0]==true && !collisions[3]) vy=-2;	y+=vy;	
			if (keysHeld() & KEY_UP && world->blocks[x/16][y/16]==LADDER && !collisions[0] && !collisions[3]) y+=-1;
			if (y>world_heightpx) hurt(3,VOID_HURT);
			if (collisions[0] && collisions[3])
				while (world->blocks[x/16][(y/16)+1] != AIR || world->blocks[x/16][y/16] != AIR)
					y-=16;
			if (animationclearframes==0) animation=0;
			else animationclearframes--;
			if (reheal>300)
			{
				if (health<20)
					health++;
				reheal=0;
			}
			showHealth(health);
		}
		else if (deathscreen==false)
		{
			deathScreenSetup();
			deathscreen=true;
			health=-50;
		}
		else
		{
			x=1024;
			y=1024;
			int result = deathScreenUpdate();
			if (result==0)
			{
				int i,j;
				for (i=0;i<=WORLD_WIDTH;i++)
					for(j=0;j<=WORLD_HEIGHT;j++)
					{
						if (canPlayerMobSpawnHere(world,i,j))
						{
							x=i*16;
							y=j*16-16;
							deathscreen=false;
							health=PLAYER_FULL_HEALTH;
							i=WORLD_WIDTH+1;
							j=WORLD_HEIGHT+1;
							animation=0;
						}
					}
			}
			else if (result==1)
			{
				quitGame=true;
				
			}
				
		}
	}
	if (x-world->CamX>-16 && x-world->CamX<256+16 && y-world->CamY>-32 && y-world->CamY<256)
	{
		if (animation==0) showGraphic(&playerMobGraphic[0],x-world->CamX - (facing ? 10:0),y-world->CamY,facing ? true:false);
		else if (animation==1) 	showGraphic(&playerMobGraphic[1],x-world->CamX - (facing ? 10:0),y-world->CamY,facing ? true:false);
	}
}
void playerMob::sendWifiUpdate()
{
}
void playerMob::saveToFile(FILE* pFile)
{
}
void playerMob::loadFromFile(FILE* pFile)
{
}
bool playerMob::isMyPlayer()
{
	return true;
}
bool canPlayerMobSpawnHere(worldObject* world,int x,int y)
{
	y++;
	if (!isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS && world->blocks[x][y+1]!=CACTUS) return true;
	return false;
}

void playerMobInit()
{
	loadGraphic(&playerMobGraphic[0],true,0);
	loadGraphic(&playerMobGraphic[1],true,1);
	loadGraphicSub(&hearts[0],0,0);
	loadGraphicSub(&hearts[1],0,1);
}

