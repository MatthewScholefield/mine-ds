#pragma once

#include "world.h"
#include "Config.h"
#define NITRO_AND_FAT

#ifdef NITRO_AND_FAT
#define USE_FAT
#define USE_NITRO
#define SHOULD_SAVE true
#define SHOULD_LOAD true
#define SOUNDBANK_FILENAME "nitro:/soundbank.bin"
#define SOUNDTRACK_FILENAME "nitro:/soundtrack.wav"
#define MINE_DS_FOLDER "fat:/Mine DS/"
#endif

#ifdef JUST_NITRO
#define USE_NITRO
#define SHOULD_SAVE false
#define SHOULD_LOAD false
#define SOUNDBANK_FILENAME "nitro:/soundbank.bin"
#define SOUNDTRACK_FILENAME "nitro:/soundtrack.wav"
#define MINE_DS_FOLDER "nitro:/Mine DS/"
#endif

#ifdef JUST_FAT
#define USE_FAT
#define SHOULD_SAVE true
#define SHOULD_LOAD true
#define SOUNDBANK_FILENAME "fat:/Mine DS/soundbank.bin"
#define SOUNDTRACK_FILENAME "fat:/Mine DS/soundtrack.wav"
#define MINE_DS_FOLDER "fat:/Mine DS/"
#endif

#define WORLD_FILENAME	"MineWorld.dat"
#define CONTROLS_FILENAME	"MineConfig.cfg"
#define TEXTURE_FOLDER "Textures/"

void initFile(void);
bool saveWorld(WorldObject *world);
bool saveConfig(Config *controls);
bool loadWorld(WorldObject *world);
bool loadConfig(Config *controls);
bool loadTexture(const char *fileName);
