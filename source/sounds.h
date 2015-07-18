#pragma once

#include "soundbank.h"  // Soundbank definitions
#include <maxmod9.h>

enum Sound
{
	SOUND_NONE = - 1,
	SOUND_COW_HURT = SFX_COW_HURT,
	SOUND_PIG_HURT = SFX_PIG_HURT,
	SOUND_PLAYER_HURT = SFX_PLAYER_HURT,
	SOUND_SHEEP_HURT = SFX_SHEEP_HURT,
	SOUND_ZOMBIE_HURT = SFX_ZOMBIE_HURT,
	SOUND_POP = SFX_POP,
	SOUND_CLICK = SFX_CLICK,
	SOUND_DOOR_CLOSE = SFX_DOOR_CLOSE,
	SOUND_DOOR_OPEN = SFX_DOOR_OPEN
};

enum SoundType
{
	SOUND_TYPE_STEP = 0,
	SOUND_TYPE_DESTROY = 1,
	SOUND_TYPE_PLACE = 1
};

enum SoundAudio
{
	SOUND_WOOD, //
	SOUND_STONE, //
	SOUND_SNOW, //
	SOUND_SAND, //
	SOUND_LADDER,
	SOUND_GRAVEL, //
	SOUND_GRASS, //
	SOUND_CLOTH
};

enum Music
{
	MUSIC_STREAM = - 2,
	MUSIC_NONE = - 1,
	MUSIC_CALM = MOD_CALM,
	MUSIC_HAL2 = MOD_HAL2
};

void initSound(void);
int getBlockPanning(int x, int camX);
void playSound(Sound sfx, mm_byte volume = 255, mm_byte panning = 127);
void playBlockSfx(int blockID, SoundType type, mm_byte volume, mm_byte panning = 127);
void playSoundNifi(Sound sfx);
void playMusic(Music song);
void stopMusic(void);
bool streamIsOpen();
