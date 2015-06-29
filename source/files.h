#pragma once

#include "world.h"
#include "Config.h"

#define WORLD_FILENAME	"MineWorld.dat"
#define CONTROLS_FILENAME	"MineConfig.cfg"
#define TEXTURE_FOLDER "Textures/"
#define MINE_DS_FOLDER "/Mine DS/"

void initFile(void);
bool saveWorld(WorldObject *world);
bool saveConfig(Config *controls);
bool loadWorld(WorldObject *world);
bool loadConfig(Config *controls);
bool loadTexture(const char *fileName);
