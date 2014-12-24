#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include "soundbank_bin.h"
#include "sounds.h"
#include "nifi.h"
#include <stdio.h>
#include <dswifi9.h>

#define LENGTH(X)	(sizeof(X) / sizeof(X[0]))

int sounds_pos = 0;
sound_t loaded_sounds[8] = { SOUND_NONE }; // circular buffer (must be power of two)
music_t loaded_music = MUSIC_NONE;

void initSound(void)
{
	mmInitDefaultMem((mm_addr) soundbank_bin);
}

static void loadSound(sound_t sound)
{
	unsigned int i;

	if (sound == SOUND_NONE)
		return;

	for (i = 0; i < LENGTH(loaded_sounds); ++i)
	{
		if (loaded_sounds[i] == sound)
			return;
	}

	if (loaded_sounds[sounds_pos] != SOUND_NONE)
		mmUnloadEffect(loaded_sounds[sounds_pos]);
	loaded_sounds[sounds_pos] = sound;
	sounds_pos = (sounds_pos + 1) & (LENGTH(loaded_sounds) - 1);
	mmLoadEffect(sound);
}

void playSound(sound_t sound)
{
	if (sound == SOUND_NONE)
		return;

	if (isWifi())
	{
		// TODO: Move this to nifi.cpp
		unsigned short buffer[100];
		int server_id = getServerID();
		sprintf((char *) buffer, "[SND: %d %d", server_id, sound);
		Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
	}

	loadSound(sound);
	mmEffect(sound);
}

void playSoundNifi(sound_t sound)
{
	loadSound(sound);
	mmEffect(sound);
}

void playMusic(music_t music)
{
	if (music == MUSIC_NONE)
		return;

	if (music != loaded_music)
	{
		stopMusic();
		mmLoad(music);
		loaded_music = music;
		mmStart(music, MM_PLAY_LOOP); //Prevents music restarting
	}
}

void stopMusic(void)
{
	if (loaded_music == MUSIC_NONE)
		return;

	mmStop();
	mmUnload(loaded_music);
	loaded_music = MUSIC_NONE;
}
