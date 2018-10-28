#include "SoundSystem.hpp"
#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9

#include "utils.hpp"
#include "audio.hpp"
#include "FileSystem.hpp"


SoundSystem *SoundSystem::instance = nullptr;

SoundSystem::SoundSystem() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
    mmInitDefault(SOUNDBANK_FILENAME);
#pragma GCC diagnostic pop

    sfxs[std::make_pair(SOUND_SNOW, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_SNOW_1, SFX_DIG_SNOW_2);
    sfxs[std::make_pair(SOUND_SNOW, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_SNOW_1, SFX_STEP_SNOW_2);
    sfxs[std::make_pair(SOUND_STONE, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_STONE_1, SFX_DIG_STONE_2);
    sfxs[std::make_pair(SOUND_STONE, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_STONE_1, SFX_STEP_STONE_2);
    sfxs[std::make_pair(SOUND_CLOTH, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_CLOTH_1, SFX_DIG_CLOTH_2);
    sfxs[std::make_pair(SOUND_CLOTH, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_CLOTH_1, SFX_STEP_CLOTH_2);
    sfxs[std::make_pair(SOUND_WOOD, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_WOOD_1, SFX_DIG_WOOD_2);
    sfxs[std::make_pair(SOUND_WOOD, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_WOOD_1, SFX_STEP_WOOD_2);
    sfxs[std::make_pair(SOUND_GRAVEL, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_GRAVEL_1, SFX_DIG_GRAVEL_2);
    sfxs[std::make_pair(SOUND_GRAVEL, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_GRAVEL_1, SFX_STEP_GRAVEL_2);
    sfxs[std::make_pair(SOUND_SAND, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_SAND_1, SFX_DIG_SAND_2);
    sfxs[std::make_pair(SOUND_SAND, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_SAND_1, SFX_STEP_SAND_2);
    sfxs[std::make_pair(SOUND_GRASS, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_GRASS_1, SFX_DIG_GRASS_2);
    sfxs[std::make_pair(SOUND_GRASS, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_GRASS_1, SFX_STEP_GRASS_2);
    sfxs[std::make_pair(SOUND_LADDER, SOUND_TYPE_DESTROY)] = std::make_pair(SFX_DIG_WOOD_1, SFX_DIG_WOOD_2);
    sfxs[std::make_pair(SOUND_LADDER, SOUND_TYPE_STEP)] = std::make_pair(SFX_STEP_LADDER_1, SFX_STEP_LADDER_2);
}

void SoundSystem::stopStream() {
    if (!streamOpen)
        return;
    mmStreamClose();
    fclose(file);
    streamOpen = false;
    loadedMusic = MUSIC_NONE;
    reqStreamClose = false;
}

bool SoundSystem::streamIsOpen() {
    if (reqStreamClose) {
        reqStreamClose = false;
        stopStream();
    }
    return streamOpen;
}

s16 SoundSystem::volumeFunc(s16 orig, u16 factor) {
    s32 n = (s32) orig * factor;
    s16 r = n / 25;
    return r;
}

mm_word SoundSystem::stream(mm_word length, mm_addr dest, mm_stream_formats format) {
    u16 musicVolume = 16;
    auto *d = (s16 *) dest;
    uint req = 0;
    while (req < length) {
        if (w.dataSize < w.blockAlign) {
            stopStream();
            return length;
        }
        *d++ = volumeFunc((s16) getADCM(file, &w), musicVolume);
        req++;
    }
    return length;
}

mm_word SoundSystem::streamFromInstance(mm_word length, mm_addr dest, mm_stream_formats format) {
    if (SoundSystem::instance) {
        return SoundSystem::instance->stream(length, dest, format);
    }
    return 0;
}

void SoundSystem::playStreamSong() {
    if (streamOpen)
        return;
    int ret = parseWave(file, &w);
    if (ret)
        return;
    ADCMReset();

    instance = this;

    mm_stream musicStream;
    musicStream.manual = 1;
    musicStream.timer = MM_TIMER1;
    musicStream.buffer_length = 1024;
    musicStream.sampling_rate = w.sampleRate;
    musicStream.format = MM_STREAM_16BIT_MONO;
    musicStream.callback = SoundSystem::streamFromInstance;

    mmStreamOpen(&musicStream);
    mmStreamUpdate();
    mmStreamUpdate();
    streamOpen = true;
}

int SoundSystem::getBlockPanning(int x, int camX) {
    return (16 * (x - ((camX + 256 / 2) / 16)) + camX % 16) + 256 / 2 + 1;
}

void SoundSystem::playSound(Sound sfx, mm_byte volume, mm_byte panning) {
    u16 sfxVolume = 16;

    if (sfx == SOUND_NONE)
        return;

    t_mmsoundeffect mobSound = {
            {(mm_word) sfx}, // id
            1024,
            0, // handle
            (mm_byte) volumeFunc(volume, sfxVolume), // volume
            panning, // panning
    };
    loadSound(sfx);
    mmEffectEx(&mobSound);
}

void SoundSystem::playMusic(Music song) {
    if (song == MUSIC_NONE)
        return;

    if (song != loadedMusic) {
        stopMusic();
        if (file != nullptr) fclose(file);
        bool canStream = (file = fopen(SOUNDTRACK_FILENAME, "rb")) != nullptr;
        if (!canStream) {
            printXY(0, 0, "MUSIC LOAD FAIL: %d", file);
            mmLoad(song);
            mmStart(song, MM_PLAY_LOOP); //Prevents music restarting
            loadedMusic = song;
        } else {
            playStreamSong();
            loadedMusic = MUSIC_STREAM;
        }
    }
}

void SoundSystem::stopMusic() {
    if (loadedMusic == MUSIC_NONE)
        return;

    if (!streamOpen && loadedMusic != MUSIC_STREAM) {
        mmStop();
        mmUnload(loadedMusic);
    } else
        stopStream();
    loadedMusic = MUSIC_NONE;
}

void SoundSystem::loadSound(int sfx) {
    unsigned int i;

    if (sfx == SOUND_NONE)
        return;

    for (i = 0; i < LENGTH(loadedSounds); ++i) {
        if (loadedSounds[i] == sfx)
            return;
    }
    if (loadedSounds[soundsPos] != SOUND_NONE)
        mmUnloadEffect(loadedSounds[soundsPos]);
    loadedSounds[soundsPos] = sfx;
    soundsPos = (soundsPos + 1) & (LENGTH(loadedSounds) - 1);
    mmLoadEffect(sfx);
}
