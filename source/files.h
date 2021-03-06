#pragma once

#include "world/World.h"
#include "Config.h"


#ifdef NITRO_AND_FAT
#define USE_FAT
#define USE_NITRO
#define SHOULD_SAVE true
#define SHOULD_LOAD true
#define SOUNDBANK_FILENAME "nitro:/soundbank.bin"
#define SOUNDTRACK_FILENAME "nitro:/soundtrack.wav"
#define MINE_DS_FOLDER "fat:/data/Mine-DS/"
#endif

#ifdef JUST_NITRO
#define USE_NITRO
#define SHOULD_SAVE false
#define SHOULD_LOAD false
#define SOUNDBANK_FILENAME "nitro:/soundbank.bin"
#define SOUNDTRACK_FILENAME "nitro:/soundtrack.wav"
#define MINE_DS_FOLDER "nitro:/Mine-DS/"
#endif

#ifdef JUST_FAT
#define USE_FAT
#define SHOULD_SAVE true
#define SHOULD_LOAD true
#define SOUNDBANK_FILENAME "fat:/data/Mine-DS/soundbank.bin"
#define SOUNDTRACK_FILENAME "fat:/data/Mine-DS/soundtrack.wav"
#define MINE_DS_FOLDER "fat:/data/Mine-DS/"
#endif

#define WORLD_FILENAME	"MineWorld.dat"
#define CONTROLS_FILENAME	"MineConfig.cfg"
#define TEXTURE_FOLDER "Textures/"

void initFile(void);
bool saveWorld(World &world);
bool saveConfig(Config *controls);
bool loadWorld(World *world);
bool loadConfig(Config *controls);
bool loadTexture(const char *fileName);
