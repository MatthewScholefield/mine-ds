#pragma once

#include "world.h"
#include "Config.h"

#define WORLD_FILENAME	"MineWorld.dat"
#define CONTROLS_FILENAME	"MineConfig.cfg"
#define MINE_DS_FOLDER "/Mine DS/"

void initFile(void);
bool saveWorld(WorldObject *world);
bool saveControls(Config *controls);
bool loadWorld(WorldObject *world);
bool loadControls(Config *controls);
