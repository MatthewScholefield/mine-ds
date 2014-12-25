#pragma once

#include "soundbank.h"  // Soundbank definitions

enum sound_t
{
	SOUND_NONE		= -1,
	SOUND_COW_HURT		= SFX_COW_HURT,
	SOUND_PIG_HURT		= SFX_PIG_HURT,
	SOUND_PLAYER_HURT	= SFX_PLAYER_HURT,
	SOUND_SHEEP_HURT	= SFX_SHEEP_HURT,
	SOUND_ZOMBIE_HURT	= SFX_ZOMBIE_HURT
};

enum music_t
{
	MUSIC_NONE	= -1,
	MUSIC_CALM	= MOD_CALM,
	MUSIC_HAL2	= MOD_HAL2
};

void initSound(void);
void playSound(sound_t sound);
void playSoundNifi(sound_t sound);
void playMusic(music_t music);
void stopMusic(void);