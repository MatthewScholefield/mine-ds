#pragma once

#include <nds/ndstypes.h>
#include <vector>
#include <nds.h>
#include <string>
#include "SkySystem.hpp"
#include "FileSystem.hpp"

#define TILES_LEN 65536
#define PAL_LEN 512

#define MOB_TILES_LEN 30720
#define MOB_PAL_LEN 512

#define TILES_ARRAY_LEN (TILES_LEN/4)
#define PAL_ARRAY_LEN (PAL_LEN/2)

#define MOB_TILES_ARRAY_LEN (MOB_TILES_LEN/4)
#define MOB_PAL_ARRAY_LEN (MOB_PAL_LEN/2)


class TitleGraphicsSystem;

struct Texture {
    struct TextureImage {
        friend class Texture;
        explicit TextureImage(size_t tilesLen, size_t palLen) : tiles(tilesLen), pal(palLen) {}

        std::vector<uint32> tiles;
        std::vector<uint16> pal;
    private:
        void read(size_t tilesLen, size_t palLen, FILE *file,
                  const unsigned int *defaultTiles, const uint16 *defaultPal);
    };

    TextureImage block{TILES_ARRAY_LEN, PAL_ARRAY_LEN};
    TextureImage mob{MOB_TILES_ARRAY_LEN, MOB_PAL_ARRAY_LEN};
    TextureImage subBg{TILES_ARRAY_LEN, PAL_ARRAY_LEN};

    void load(FILE *file);
    void loadDefault();
};

class GraphicsSystem {
public:
    GraphicsSystem();
    void bind(TitleGraphicsSystem &titleGraphics);
    void renderBlock(int i, int j, int blockId);
    void beginRender(int screenX, int screenY);

    uint16 *getMainBgPtr() const;
    uint16 *getSubBgPtr() const;
    int getSubBgID() const;
    const Texture &getTexture() const;

private:
    void updateTexture();
    inline void setTileXY(int x, int y, int tile, int palette);
    void renderTile16(int x, int y, int tile, int palette);
    bool loadTextureFile(const std::string &fileName, bool reloadGfx = true);
    void setBlockPalette(bool blocks, int brightness, int index);

    Texture texture;
    uint16 *subBgPtr, *mainBgPtr;
    int subBgID, mainBgID;
    SkySystem sky;
    TitleGraphicsSystem *titleGraphics;
};
