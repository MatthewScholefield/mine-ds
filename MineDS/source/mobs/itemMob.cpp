#include <stdio.h>
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

itemMob::itemMob(int a,int b)
{
  x = a;
  y = b;
  health = 1;
  mobtype = 8;
}
bool itemMob::isMyPlayer()
{
  return false;
}
void itemMob::updateMob(worldObject* world)
{
  baseMob* target;
  target = mobHandlerFindMob(256, 2, x, y);
  if (target->isMyPlayer())
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
