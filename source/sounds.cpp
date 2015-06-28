#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include "sounds.h"
#include "nifi.h"
#include "blockID.h"
#include "blocks.h"
#include "mining.h"
#include <stdio.h>
#include <dswifi9.h>
#include <map>

#include "general.h"
#include "audio/audio.h"
#define LENGTH(X)	(sizeof(X) / sizeof(X[0]))

int soundsPos = 0;
int loadedSounds[8] = {SOUND_NONE}; // circular buffer (must be power of two)
Music loadedMusic = MUSIC_NONE;
bool streamOpen = false;
std::map< std::pair< SoundAudio, SoundType >, std::pair< int, int > > sfxs;

FILE *file;
waveInfo w;

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
	return loadedMusic != MUSIC_NONE;
}

mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format)
{
  s16* d = (s16*)dest;	
  int req = 0;
  while (req < length)
  {
    if (feof(file))
    {
			mmStreamClose();
			fclose(file);
      streamOpen = false;
      return req; 
    }
    *d++ = (s16)getADCM(file,&w);
    req++;
  }
	return length;
}

void playStreamSong()
{
	if (streamOpen)
		return;
  int ret =  parseWave(file,&w);
  if (ret)
    return;
  
	mm_stream musicStream;
	musicStream.buffer_length = 1024;
	musicStream.callback = stream;
	musicStream.timer = MM_TIMER1;
	musicStream.manual = true;

	musicStream.sampling_rate = w.sampleRate;
	musicStream.format = MM_STREAM_16BIT_MONO;
	mmStreamOpen(&musicStream);
	streamOpen = true;
}

void initSound(void)
{
#pragma GCC diagnostic ignored "-Wwrite-strings"
	mmInitDefault("nitro:/soundbank.bin");
#pragma GCC diagnostic pop

	sfxs[ std::make_pair(SOUND_SNOW, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_SNOW_1, SFX_DIG_SNOW_2);
	sfxs[ std::make_pair(SOUND_SNOW, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_SNOW_1, SFX_STEP_SNOW_2);
	sfxs[ std::make_pair(SOUND_STONE, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_STONE_1, SFX_DIG_STONE_2);
	sfxs[ std::make_pair(SOUND_STONE, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_STONE_1, SFX_STEP_STONE_2);
	sfxs[ std::make_pair(SOUND_CLOTH, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_CLOTH_1, SFX_DIG_CLOTH_2);
	sfxs[ std::make_pair(SOUND_CLOTH, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_CLOTH_1, SFX_STEP_CLOTH_2);
	sfxs[ std::make_pair(SOUND_WOOD, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_WOOD_1, SFX_DIG_WOOD_2);
	sfxs[ std::make_pair(SOUND_WOOD, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_WOOD_1, SFX_STEP_WOOD_2);
	sfxs[ std::make_pair(SOUND_GRAVEL, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_GRAVEL_1, SFX_DIG_GRAVEL_2);
	sfxs[ std::make_pair(SOUND_GRAVEL, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_GRAVEL_1, SFX_STEP_GRAVEL_2);
	sfxs[ std::make_pair(SOUND_SAND, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_SAND_1, SFX_DIG_SAND_2);
	sfxs[ std::make_pair(SOUND_SAND, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_SAND_1, SFX_STEP_SAND_2);
	sfxs[ std::make_pair(SOUND_GRASS, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_GRASS_1, SFX_DIG_GRASS_2);
	sfxs[ std::make_pair(SOUND_GRASS, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_GRASS_1, SFX_STEP_GRASS_2);
	sfxs[ std::make_pair(SOUND_LADDER, SOUND_TYPE_DESTROY) ] = std::make_pair(SFX_DIG_WOOD_1, SFX_DIG_WOOD_2);
	sfxs[ std::make_pair(SOUND_LADDER, SOUND_TYPE_STEP) ] = std::make_pair(SFX_STEP_LADDER_1, SFX_STEP_LADDER_2);
}

static void loadSound(int sfx)
{
	unsigned int i;

	if (sfx == SOUND_NONE)
		return;

	for (i = 0; i < LENGTH(loadedSounds); ++i)
	{
		if (loadedSounds[i] == sfx)
			return;
	}
	if (loadedSounds[soundsPos] != SOUND_NONE)
		mmUnloadEffect(loadedSounds[soundsPos]);
	loadedSounds[soundsPos] = sfx;
	soundsPos = (soundsPos + 1) & (LENGTH(loadedSounds) - 1);
	mmLoadEffect(sfx);
}

void playSound(Sound sfx)
{
	if (sfx == SOUND_NONE)
		return;

	if (isWifi())
	{
		// TODO: Move this to nifi.cpp
		unsigned short buffer[100];
		int server_id = getServerID();
		sprintf((char *) buffer, "[SND: %d %d", server_id, sfx);
		Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
	}

	loadSound(sfx);
	mmEffect(sfx);
}

int getSfx(int blockID, SoundType sound)
{
	const auto key = std::make_pair(getBlockAudio(blockID), sound);
	if (sfxs.find(key) != sfxs.end())
	{
		return ( rand() % 2) ? (sfxs[ key ].first) : (sfxs[ key ].second);
	}
	return -1;
}

void playBlockSfx(int blockID, SoundType type, mm_byte volume, mm_byte panning)
{
	if (blockID == AIR)
		return;
	int sfxID = getSfx(blockID, type);
	t_mmsoundeffect blockSound = {
		{(mm_word) sfxID}, // id
		1024,
		0, // handle
		volume, // volume
		panning, // panning
	};
	loadSound(sfxID);
	mmEffectEx(&blockSound);
}

void playSoundNifi(Sound sfx)
{
	loadSound(sfx);
	mmEffect(sfx);
}

void playMusic(Music song)
{
	if (song == MUSIC_NONE)
		return;

	if (song != loadedMusic)
	{
		stopMusic();
		bool canStream = (file = fopen("nitro:/soundtrack.wav", "rb")) != NULL;
		if (!canStream)
		{
			mmLoad(song);
			mmStart(song, MM_PLAY_LOOP); //Prevents music restarting
		}
		else{
			playStreamSong();
    }
		loadedMusic = song;
	}
}

void stopMusic(void)
{
	if (loadedMusic == MUSIC_NONE)
		return;

	if (!streamOpen)
	{
		mmStop();
		mmUnload(loadedMusic);
	}
	else
		stopStream();
	loadedMusic = MUSIC_NONE;
}
