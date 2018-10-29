#pragma once

#include <nds/ndstypes.h>
#include <vector>
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

class GraphicsSystem {
public:
    GraphicsSystem();
    void bind(TitleGraphicsSystem &titleGraphics);
    void renderBlock(int i, int j, int blockId);
    void beginRender(int screenX, int screenY);

    uint16 *getMainBgPtr() const;
    uint16 *getSubBgPtr() const;
    int getSubBgID() const;

    u8 *getMobTiles() const;
    u8 *getSubBgTiles() const;
    u8 *getBlockTiles() const;
    u8 *getBlockPal() const;

private:
    void loadTexture(const unsigned int *blockTilesSrc, const unsigned short *blockPalSrc,
                     const unsigned int *mobTilesSrc, const unsigned short *mobPalSrc,
                     const unsigned int *subBgTilesSrc, const unsigned short *subBgPalSrc, bool skipReload = false);
    void updateTexture();
    void loadDefaultTexture();
    inline void setTileXY(int x, int y, int tile, int palette);
    void renderTile16(int x, int y, int tile, int palette);
    bool loadTextureFile(const char *fileName);
    void setBlockPalette(bool blocks, int brightness, int index);

    std::vector<unsigned int> blockTiles, mobTiles, subBgTiles;
    std::vector<unsigned short> blockPal, mobPal, subBgPal;
    uint16 *subBgPtr, *mainBgPtr;
    int subBgID, mainBgID;
    SkySystem sky;
    TitleGraphicsSystem *titleGraphics;
};
