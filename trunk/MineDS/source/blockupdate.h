#pragma once
#include "world.h"

class blockUpdater
{
  public:
    int blockID = 15;
    blockUpdater();
    int chance;
    virtual int chanceUpdate(worldObject* world,int x,int y,bool bg);
    virtual void update(worldObject* world,int x,int y,bool bg);
};
void proceduralBlockUpdateInit();
void proceduralBlockUpdate(worldObject* world);
void checkBlockPlace(int x,int y,worldObject* world,bool bg);
void checkBlockDelete(int x,int y,worldObject* world,bool bg);
