#include <stdio.h>
#include <math.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "itemMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../general.h"
#include "../sounds.h"
#include "../collision.h"
#include "../inventory.h"

#include <nds.h>
#include <stdio.h>
#include "../blocks.h"
#include "../world.h"
#include "../fontHandler.h"
#include "../worldgen.h"
#include "../mining.h"
#include "../blocks.h"
#include "../worldRender.h"
#include "../titlescreen.h"
#include "../mainGame.h"
#include "../graphics/graphics.h"
#include "../graphics/subBgHandler.h"
#include "../crafting.h"
#include "../nifi.h"
#include "../sounds.h"
#include "../files.h"
#include "../general.h"
#include "../blockupdate.h"
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include <string>

itemMob::itemMob(int a,int b)
{
	noPhysics = false;
	sx = 4;
	sy = 4;
  x = a;
  y = b;
  health = 1;
  mobtype = 8;
  blockID = GLOWSTONE;
  amount = 1;
  floatY = 0;
  loadedBlockGfx = false;
}
bool itemMob::isMyPlayer()
{
  return false;
}

void itemMob::updateMob(worldObject* world)
{
	if (!alive)
		return;
	if (!loadedBlockGfx)
	{
		loadGraphicMiniBlock(&itemGraphic, blockID,8,8);
		loadedBlockGfx = true;
	}
	++floatY;
	if (floatY>100)
		floatY=0;
  baseMob* target;
  target = mobHandlerFindMob(8, 2, x, y-8);
  if (target==NULL)
	  target = mobHandlerFindMob(8, 2, x, y-24);
  if (target == NULL || !target->isMyPlayer())
	  showGraphic(&itemGraphic, x - world->CamX, y - world->CamY-4+int(4.0*sin(double(floatY)*6.28/100)),false);
  else
  {
    addInventory(blockID, amount);
    alive = false;
    killMob();
  }
}

void itemMob::sendWifiUpdate()
{
}

void itemMob::saveToFile(FILE* pFile)
{
}

void itemMob::loadFromFile(FILE* pFile)
{
  killMob();
}
void itemMob::hurt(int hamount, int type)
{
   blockID = hamount;
   amount = type;
}
