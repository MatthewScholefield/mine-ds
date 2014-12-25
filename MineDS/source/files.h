#pragma once

#include "world.h"
#include "Config.h"

#define WORLD_PATH	"/MineData.bin"
#define CONTROLS_PATH	"/MineConfig.bin"

void initFile(void);
bool saveWorld(worldObject *world);
bool saveControls(Config *controls);
bool loadWorld(worldObject *world);
bool loadControls(Config *controls);
