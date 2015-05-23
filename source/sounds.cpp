#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include "soundbank_bin.h"
#include "sounds.h"
#include "nifi.h"
#include <stdio.h>
#include <dswifi9.h>

#define LENGTH(X)	(sizeof(X) / sizeof(X[0]))

int sounds_pos = 0;
sound_t loaded_sounds[8] = {SOUND_NONE}; // circular buffer (must be power of two)
music_t loaded_music = MUSIC_NONE;
bool streamOpen = false;

FILE *file;

void stopStream()
{
	if (!streamOpen)
		return;
	mmStreamClose();
	fclose(file);
	streamOpen = false;
}

bool streamIsOpen()
{
	return streamOpen;
}

bool songIsPlaying()
{
	return loaded_music != MUSIC_NONE;
}

mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format)
{
	if (file)
	{
		size_t samplesize = 1;
		switch (format)
		{
			case MM_STREAM_8BIT_MONO: samplesize = 1;
				break;
			case MM_STREAM_8BIT_STEREO: samplesize = 2;
				break;
			case MM_STREAM_16BIT_MONO: samplesize = 2;
				break;
			case MM_STREAM_16BIT_STEREO: samplesize = 4;
				break;
		}

		int res = fread(dest, samplesize, length, file);
		if (res)
		{
			length = res;
		}
		else
		{
			mmStreamClose();
			fclose(file);
			length = 0;
			streamOpen = false;
		}
	}
	return length;
}

void playStreamSong()
{
	if (streamOpen)
		return;
	mm_stream mystream;
	mystream.buffer_length = 1024;
	mystream.callback = stream;
	mystream.timer = MM_TIMER1;
	mystream.manual = true;

	mystream.sampling_rate = 22050;
	mystream.format = MM_STREAM_16BIT_STEREO;
	mmStreamOpen(&mystream);
	mmStreamUpdate();
	mmStreamUpdate();
	streamOpen = true;
}

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
		bool canStream = (file = fopen("MineCraft Remix - Calm 3 Chillstep.raw", "rb")) != NULL;
		if (!canStream)
		{
			mmLoad(music);
			mmStart(music, MM_PLAY_LOOP); //Prevents music restarting
		}
		else
			playStreamSong();
		loaded_music = music;
	}
}

void stopMusic(void)
{
	if (loaded_music == MUSIC_NONE)
		return;

	if (!streamOpen)
	{
		mmStop();
		mmUnload(loaded_music);
	}
	else
		stopStream();
	loaded_music = MUSIC_NONE;
}
