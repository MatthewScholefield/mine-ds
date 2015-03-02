#pragma once
#include "world.h"
#define NO_CHANCE 99999
#define SOIL_CHANCE_UPDATE 1000
#define LEAF_CHANCE_UPDATE 800
#define LEAF_SQUARE_RADIUS 4
class blockUpdater
{
  public:
    int blockID = 15;
    blockUpdater();
    int chance = NO_CHANCE;
    virtual void chanceUpdate(worldObject* world,int x,int y,bool bg);
    virtual void update(worldObject* world,int x,int y,bool bg);
};
void proceduralBlockUpdateInit();
void proceduralBlockUpdate(worldObject* world);
void checkBlockPlace(int x,int y,worldObject* world,bool bg);
void checkBlockDelete(int x,int y,worldObject* world,bool bg);
