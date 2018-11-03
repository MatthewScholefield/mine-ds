#pragma once

#include <vector>
#include <nds.h>
#include "Graphics.hpp"


struct Texture {
    static constexpr int palLen = 512;

    struct TextureImage {
        friend class Texture;

        TextureImage(unsigned long tilesBytes, unsigned long palBytes);

        std::vector<uint32_t> tiles;
        std::vector<uint16_t> pal;
    private:
        void read(FILE *file, const unsigned int *defaultTiles, const uint16_t *defaultPal);
    };

    Texture();

    TextureImage block, mob, subBg;

    void load(FILE *file);
    void loadDefault();
};
