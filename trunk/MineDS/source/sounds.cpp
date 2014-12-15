#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include "soundbank_bin.h"
#include "sounds.h"
#include "nifi.h"
#include <stdio.h>
#include <dswifi9.h>
#include <string.h>

music_t last_music = MUSIC_NONE;

void initSound(void)
{
	unsigned int i;

	mmInitDefaultMem((mm_addr) soundbank_bin);
	for (i = 0; i < MSL_NSAMPS; i++)
		mmLoadEffect(i); // loads all sfx
}

void playSound(sound_t sound)
{
	if (isWifi())
	{
		// TODO: Move this to nifi.cpp
		unsigned short buffer[100];
		int server_id = getServerID();
		sprintf((char *) buffer, "[SND: %d %d", server_id, sound);
		Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
	}
	mmEffect(sound);
}

void playSoundNifi(sound_t sound)
{
	mmEffect(sound);
}

void playMusic(music_t music)
{
	if (music == MUSIC_NONE)
		return;

	if (music != last_music) {
		stopMusic();
		mmLoad(music);
		last_music = music;
	}
	mmStart(music, MM_PLAY_LOOP);
}

void stopMusic(void)
{
	if (last_music == MUSIC_NONE)
		return;

	mmStop();
	mmUnload(last_music);
	last_music = MUSIC_NONE;
}
