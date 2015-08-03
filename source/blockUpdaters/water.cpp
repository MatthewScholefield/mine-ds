#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include "../blockID.h"
#include "../blocks.h"
#include "water.h"
#include "../mobs/mobHandler.h"
//	createItemMob(x, y, getSurvivalItem(leafID));
//    ^^ Used as reference when writing this file
WaterUpdater::WaterUpdater()
{
	blockID = WATER;
	chance = NO_CHANCE;
}
void WaterUpdater::attemptSpreading(WorldObject* world, int x, int y)
{
  int waterLevel = world->data[x][y] & 0xF;
  if (waterLevel > 12) waterLevel = 12;
  bool rightClear = false;
  bool leftClear = false;
  bool rightDownSolid = false;
  bool leftDownSolid = false;
  bool downClear = false;
  if (x > 1)
  {
    leftClear = world->blocks[x-1][y] == AIR;
    if (y < WORLD_HEIGHT-1)
    {
      if (world->blocks[x-1][y+1] == WATER || !isBlockWalkThrough(world->blocks[x-1][y+1]))
        leftDownSolid = true;
    }
  }
  if (x < WORLD_WIDTH-1)
  {
    rightClear = world->blocks[x+1][y] == AIR;
    if (y < WORLD_HEIGHT-1)
    {
      if (world->blocks[x+1][y+1] == WATER || !isBlockWalkThrough(world->blocks[x+1][y+1]))
        rightDownSolid = true;
    }
  }
  if (y < WORLD_HEIGHT-1)
  {
    downClear = isBlockWalkThrough(world->blocks[x][y+1]) && world->blocks[x][y+1]!=WATER;
  }
  
  //With that out of the way we can check to see if we _can_ spread
  bool spreadRight = false;
  bool spreadLeft = false;
  if (!downClear)
  {
    if (rightClear)
      spreadRight = true;
    if (leftClear) 
      spreadLeft = true;
  }
  if (rightClear && rightDownSolid) spreadRight = true;
  if (leftClear && leftDownSolid) spreadLeft = true;  
  
  //Now check that we have enough water for that spreading
  int reqDiv = 1;
  if (spreadLeft) ++reqDiv;
  if (spreadRight) ++reqDiv;
    //If there isn't enough water to spread, don't
  if (waterLevel < reqDiv)
  {
    printLocalMessage("Not enough water to spread");
    return;
  }

  //Otherwise, split at will
  int middleLevel = (waterLevel / reqDiv) + (waterLevel % reqDiv);
  int otherLevels = (waterLevel / reqDiv);
  if (spreadLeft)
  {
    createItemMob(x-1, y, getSurvivalItem(world->blocks[x][y]));
    world->blocks[x-1][y] = WATER;
    world->data[x-1][y] = (world->data[x-1][y]&0xFFFF0000) | otherLevels;
  }
  if (spreadRight)
  {
    createItemMob(x+1, y, getSurvivalItem(world->blocks[x][y]));
    world->blocks[x+1][y] = WATER;
    world->data[x+1][y] = (world->data[x-1][y]&0xFFFF0000) | otherLevels;
  }
  //Update this water blocks water level
  world->data[x][y]&= 0xFFFF0000;
  world->data[x][y]|=middleLevel;
}
void WaterUpdater::update(WorldObject* world, int x, int y, bool bg)
{
  if (bg == true)
  {
    //Try to not have water in the background
    if (world->blocks[x][y]==AIR)
    {
      world->blocks[x][y]=WATER;
    }
    world->bgblocks[x][y]=AIR;
  }
  else
  {
    if (y < (WORLD_HEIGHT - 1) && isBlockWalkThrough(world->blocks[x][y+1]) && world->blocks[x][y+1]!=WATER)
    {
      world->blocks[x][y+1]=WATER;
      world->data[x][y+1] = (world->data[x][y] & 0xFFFF) | (world->data[x][y+1] & 0xFFFF0000);
      world->blocks[x][y]=AIR;
    }
    else
    {
      attemptSpreading(world,x,y);
    }
  }
}
