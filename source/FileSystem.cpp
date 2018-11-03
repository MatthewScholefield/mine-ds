#include <fat.h>
#include <nds.h>
#include <string>
#include <sys/stat.h>
#include <filesystem.h>
#include "utils.hpp"
#include "FileSystem.hpp"
#include "graphics/Graphic.hpp"
#include "graphics/Graphics.hpp"

void initFile() {
#ifdef USE_NITRO
    nitroFSInit(nullptr);
    chdir("nitro:/");
#endif
#ifdef USE_FAT
    fatInitDefault();
    chdir("fat:/");
    mkdir("data", 0777);
    chdir("fat:/data");
    mkdir("Mine-DS", 0777);
    chdir("fat:/data/Mine-DS");
    mkdir("Textures", 0777);
    chdir("fat:/");
#endif
}
