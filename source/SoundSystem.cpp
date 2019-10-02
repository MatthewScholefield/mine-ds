#include "SoundSystem.hpp"
#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9

#include "utils.hpp"
#include "FileSystem.hpp"


SoundSystem *SoundSystem::instance = nullptr;

SoundSystem::SoundSystem() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
    mmInitDefault(SOUNDBANK_FILENAME);
#pragma GCC diagnostic pop

    sfxs[std::make_pair(SoundAudio::Snow, SoundType::Destroy)] = std::make_pair(SFX_DIG_SNOW_1, SFX_DIG_SNOW_2);
    sfxs[std::make_pair(SoundAudio::Snow, SoundType::Step)] = std::make_pair(SFX_STEP_SNOW_1, SFX_STEP_SNOW_2);
    sfxs[std::make_pair(SoundAudio::Stone, SoundType::Destroy)] = std::make_pair(SFX_DIG_STONE_1, SFX_DIG_STONE_2);
    sfxs[std::make_pair(SoundAudio::Stone, SoundType::Step)] = std::make_pair(SFX_STEP_STONE_1, SFX_STEP_STONE_2);
    sfxs[std::make_pair(SoundAudio::Cloth, SoundType::Destroy)] = std::make_pair(SFX_DIG_CLOTH_1, SFX_DIG_CLOTH_2);
    sfxs[std::make_pair(SoundAudio::Cloth, SoundType::Step)] = std::make_pair(SFX_STEP_CLOTH_1, SFX_STEP_CLOTH_2);
    sfxs[std::make_pair(SoundAudio::Wood, SoundType::Destroy)] = std::make_pair(SFX_DIG_WOOD_1, SFX_DIG_WOOD_2);
    sfxs[std::make_pair(SoundAudio::Wood, SoundType::Step)] = std::make_pair(SFX_STEP_WOOD_1, SFX_STEP_WOOD_2);
    sfxs[std::make_pair(SoundAudio::Gravel, SoundType::Destroy)] = std::make_pair(SFX_DIG_GRAVEL_1, SFX_DIG_GRAVEL_2);
    sfxs[std::make_pair(SoundAudio::Gravel, SoundType::Step)] = std::make_pair(SFX_STEP_GRAVEL_1, SFX_STEP_GRAVEL_2);
    sfxs[std::make_pair(SoundAudio::Sand, SoundType::Destroy)] = std::make_pair(SFX_DIG_SAND_1, SFX_DIG_SAND_2);
    sfxs[std::make_pair(SoundAudio::Sand, SoundType::Step)] = std::make_pair(SFX_STEP_SAND_1, SFX_STEP_SAND_2);
    sfxs[std::make_pair(SoundAudio::Grass, SoundType::Destroy)] = std::make_pair(SFX_DIG_GRASS_1, SFX_DIG_GRASS_2);
    sfxs[std::make_pair(SoundAudio::Grass, SoundType::Step)] = std::make_pair(SFX_STEP_GRASS_1, SFX_STEP_GRASS_2);
    sfxs[std::make_pair(SoundAudio::Ladder, SoundType::Destroy)] = std::make_pair(SFX_DIG_WOOD_1, SFX_DIG_WOOD_2);
    sfxs[std::make_pair(SoundAudio::Ladder, SoundType::Step)] = std::make_pair(SFX_STEP_LADDER_1, SFX_STEP_LADDER_2);
}

void SoundSystem::stopStream() {
    if (!streamOpen) {
        return;
    }
    mmStreamClose();
    fclose(file);
    streamOpen = false;
    loadedMusic = Music::None;
    reqStreamClose = false;
}

void SoundSystem::update() {
    if (reqStreamClose) {
        reqStreamClose = false;
        stopStream();
    }
    if (streamOpen) {
        mmStreamUpdate();
    }
}

s16 SoundSystem::volumeFunc(s16 orig, u16 factor) {
    return s16(s32(orig * factor) / 25);
}

mm_word SoundSystem::stream(mm_word length, mm_addr dest, mm_stream_formats format) {
    auto *d = (s16 *) dest;
    for (uint req = 0; req < length; ++req) {
        if (nibbleReader.isDone()) {
            stopStream();
            return req;
        }
        *d++ = adpcmProcessor.decode(nibbleReader.get());
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
    if (streamOpen) {
        return;
    }
    
    nibbleReader.reset();
    adpcmProcessor.reset();

    instance = this;

    mm_stream musicStream;
    musicStream.manual = 1;
    musicStream.timer = MM_TIMER1;
    musicStream.buffer_length = MM_BUFFER_SIZE;
    musicStream.sampling_rate = 22050;
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

    if (sfx == Sound::None) {
        return;
    }

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
    if (song == Music::None) {
        return;
    }
    if (song != loadedMusic) {
        stopMusic();
        if (file != nullptr) fclose(file);
        bool canStream = (file = fopen(SOUNDTRACK_FILENAME, "rb")) != nullptr;
        if (!canStream) {
            printXY(0, 0, "MUSIC LOAD FAIL: %d", file);
            mmLoad((mm_word) song);
            mmStart((mm_word) song, MM_PLAY_LOOP); //Prevents music restarting
            loadedMusic = song;
        } else {
            nibbleReader.setFile(file);
            playStreamSong();
            loadedMusic = Music::Stream;
        }
    }
}

void SoundSystem::stopMusic() {
    if (loadedMusic == Music::None) {
        return;

    }
    if (!streamOpen && loadedMusic != Music::Stream) {
        mmStop();
        mmUnload((mm_word) loadedMusic);
    } else
        stopStream();
    loadedMusic = Music::None;
}

void SoundSystem::loadSound(Sound sfx) {
    unsigned int i;

    if (sfx == Sound::None) {
        return;
    }
    for (i = 0; i < LENGTH(loadedSounds); ++i) {
        if (loadedSounds[i] == sfx) {
            return;
        }
    }
    if (loadedSounds[soundsPos] != Sound::None) {
        mmUnloadEffect((mm_word) loadedSounds[soundsPos]);
    }
    loadedSounds[soundsPos] = sfx;
    soundsPos = (soundsPos + 1) & (LENGTH(loadedSounds) - 1);
    mmLoadEffect((mm_word) sfx);
}
