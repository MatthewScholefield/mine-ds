#include <nds.h>
#include <font.h>
#include <block_small.h>
#include <sub_bg.h>
#include <mobs.h>
#include <particles.h>
#include <sub.h>
#include "GraphicsSystem.hpp"
#include "subBgHandler.hpp"
#include "Graphic.hpp"
#include "FileSystem.hpp"
#include "TitleGraphicsSystem.hpp"
#include "TitleFontSystem.hpp"
#include <string>

GraphicsSystem::GraphicsSystem() : titleGraphics(nullptr) {
    videoSetModeSub(MODE_5_2D | DISPLAY_BG_EXT_PALETTE);

    vramSetBankC(VRAM_C_SUB_BG);
    subBgID = bgInitSub(2, BgType_ExRotation, BgSize_ER_512x512, 4, 6); //16bit BG
    bgSetScroll(subBgID, 0, -64);
    bgWrapOn(subBgID);
    subBgPtr = bgGetMapPtr(subBgID);
    vramSetBankH(VRAM_H_LCD);
    dmaCopy(&fontPal, VRAM_H_EXT_PALETTE[0][0], fontPalLen); //Copy the palette
    vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
    dmaCopy(&fontTiles, (uint16 *) 0x0620400, fontTilesLen);

    // drawBackground() Missing

    // Main Graphics (World)
    loadTextureFile("default");
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    REG_DISPCNT = MODE_5_3D | DISPLAY_BG_EXT_PALETTE;
    mainBgID = bgInit(2, BgType_ExRotation, BgSize_ER_512x512, 0x10, 0);
    bgShow(mainBgID);
    bgSetPriority(mainBgID, 1);
    REG_BG2CNT |= BG_WRAP_ON;
    bgUpdate();
    mainBgPtr = bgGetMapPtr(mainBgID); //The Map Base
    for (int i = 0; i <= 16; ++i) {
        for (int j = 0; j <= 16; ++j) {
            renderTile16(i, j, 0, 0);
        }
    }

    // Main Graphics (Other)
    sky.setSkyColor(0, 29, 31, 0, 10, 31);

    vramSetBankD(VRAM_D_SUB_SPRITE);

    //Set the bank for our Graphics.
    oamInit(&oamSub, SpriteMapping_1D_256, true);
    //Vram I is for Sub Sprite Palette!
    vramSetBankI(VRAM_I_LCD);
    dmaCopy(subPal, VRAM_I_EXT_SPR_PALETTE[0], subPalLen);
    dmaCopy(fontPal, VRAM_I_EXT_SPR_PALETTE[1], fontPalLen);
    vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
    vramSetBankB(VRAM_B_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_256, true);
    Graphic::resetSprites(true);
    Graphic::resetSprites(false);
}

void GraphicsSystem::bind(TitleGraphicsSystem &titleGraphics) {
    this->titleGraphics = &titleGraphics;
}

void GraphicsSystem::loadTexture(const unsigned int *blockTilesSrc, const unsigned short *blockPalSrc,
                                 const unsigned int *mobTilesSrc, const unsigned short *mobPalSrc,
                                 const unsigned int *subBgTilesSrc, const unsigned short *subBgPalSrc,
                                 bool skipReload) {
    if (!blockTilesSrc || !blockPalSrc) {
        blockTilesSrc = block_smallTiles;
        blockPalSrc = block_smallPal;
    }

    if (!subBgTilesSrc || !subBgPalSrc) {
        subBgTilesSrc = sub_bgTiles;
        subBgPalSrc = sub_bgPal;
    }

    if (!mobTilesSrc || !mobPalSrc) {
        mobTilesSrc = mobsTiles;
        mobPalSrc = mobsPal;
    }

    blockTiles.assign(blockTilesSrc, blockTilesSrc + TILES_ARRAY_LEN);
    blockPal.assign(blockPalSrc, blockPalSrc + PAL_ARRAY_LEN);

    subBgTiles.assign(subBgTilesSrc, subBgTilesSrc + TILES_ARRAY_LEN);
    subBgPal.assign(subBgPalSrc, subBgPalSrc + PAL_ARRAY_LEN);

    mobTiles.assign(mobTilesSrc, mobTilesSrc + MOB_TILES_ARRAY_LEN);
    mobPal.assign(mobPalSrc, mobPalSrc + MOB_PAL_ARRAY_LEN);

    if (!skipReload) {
        ++Graphic::textureID;
    }
}

void GraphicsSystem::loadDefaultTexture() {
    loadTexture(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, true);
}

void GraphicsSystem::updateTexture() {
    const int NUM_BANK_SLOTS = 16;
    const int MAX_BRIGHTNESS = 15;
    const int NUM_PALETTE_COLORS = 256;
    const int BLOCK_PAL_BG = 2;
    //=== Main Block BG ===
    vramSetBankE(VRAM_E_LCD);
    dmaCopy(blockPal.data(), VRAM_E_EXT_PALETTE[BLOCK_PAL_BG][NUM_BANK_SLOTS - 1], PAL_LEN); //Copy the palette
    for (int i = 0; i < NUM_BANK_SLOTS; ++i) {
        for (int j = 0; j < NUM_PALETTE_COLORS; ++j) {
            uint16 col = VRAM_E_EXT_PALETTE[BLOCK_PAL_BG][NUM_BANK_SLOTS - 1][j];
            auto r = uint16((col >> 0) & 0x1F);
            auto g = uint16((col >> 5) & 0x1F);
            auto b = uint16((col >> 10) & 0x1F);
            auto a = uint16((col >> 15) & 0x1);
            r = uint16((r * i) / (NUM_BANK_SLOTS - 1));
            g = uint16((g * i) / (NUM_BANK_SLOTS - 1));
            b = uint16((b * i) / (NUM_BANK_SLOTS - 1));
            VRAM_E_EXT_PALETTE[2][i][j] = r << 0 |
                                          g << 5 |
                                          b << 10 |
                                          a << 15;
        }
    }
    vramSetBankE(VRAM_E_BG_EXT_PALETTE);


    vramSetBankF(VRAM_F_LCD);
    for (int i = 1; i <= NUM_BANK_SLOTS / 2; ++i) {
        //=== Main Block Gfx ===
        setBlockPalette(true, (MAX_BRIGHTNESS * i) / (NUM_BANK_SLOTS / 2), i - 1);
        //=== Mob Sprite Gfx ===
        if (i != NUM_BANK_SLOTS / 2)//Save room for the particle palette at index 15
            setBlockPalette(false, (MAX_BRIGHTNESS * i) / (NUM_BANK_SLOTS / 2 - 1), i - 1 + NUM_BANK_SLOTS / 2);
    }
    //=== Particles ===
    dmaCopy(particlesPal, VRAM_F_EXT_SPR_PALETTE[15], particlesPalLen);
    vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);

    //=== Sub Block ===
    vramSetBankI(VRAM_I_LCD);
    dmaCopy(blockPal.data(), VRAM_I_EXT_SPR_PALETTE[2], PAL_LEN);
    vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
    dmaCopy(blockTiles.data(), bgGetGfxPtr(mainBgID), TILES_LEN);

    //=== Sub BG ===
    vramSetBankH(VRAM_H_LCD);
    dmaCopy(subBgPal.data(), VRAM_H_EXT_PALETTE[2][0], PAL_LEN);
    vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
    dmaCopy(subBgTiles.data(), bgGetGfxPtr(subBgID), sub_bgTilesLen);
    if (titleGraphics) {
        titleGraphics->getTitleFont().refresh();
        titleGraphics->updateSubBG();
    }

    swiWaitForVBlank(); //Prevents sub screen flicker
}

void GraphicsSystem::setBlockPalette(bool blocks, int brightness, int index) {
    auto *palette = new unsigned short[PAL_LEN / 2];
    for (int i = 0; i < PAL_LEN / 2; ++i) {
        uint16 slot = blocks ? blockPal[i] : mobPal[i];
        auto blue = uint16(slot & 0x1F);
        slot >>= 5;
        auto green = uint16(slot & 0x1F);
        slot >>= 5;
        unsigned short red = slot;
        blue = uint16(blue * brightness / 15);
        green = uint16(green * brightness / 15);
        red = uint16(red * brightness / 15); //*/
        slot = red;
        slot <<= 5;
        slot |= green;
        slot <<= 5;
        slot |= blue;
        palette[i] = slot;
    }
    DC_FlushRange(palette, PAL_LEN);
    dmaCopy(palette, VRAM_F_EXT_SPR_PALETTE[index], PAL_LEN);
    delete[] palette;
}

void GraphicsSystem::setTileXY(int x, int y, int tile, int palette) {
    tile |= palette << 12;
    mainBgPtr[(x % 64) + (y % 64) * 64] = (uint16) tile;
}

void GraphicsSystem::renderTile16(int x, int y, int tile, int palette) {
    x *= 2; //Convert tiles of 16 to tiles of 8
    y *= 2;
    tile *= 2 * 2;
    if (palette < 0) palette = 1;
    //Draw the 4 (8x8) tiles
    setTileXY(x, y, tile, palette);
    setTileXY(x + 1, y, tile + 2, palette);
    setTileXY(x, y + 1, tile + 1, palette);
    setTileXY(x + 1, y + 1, tile + 3, palette);
}

void GraphicsSystem::renderBlock(int i, int j, int blockId) {
    int brightness = 15;
    renderTile16(i, j, blockId, brightness);
}

void GraphicsSystem::beginRender(int screenX, int screenY) {
    bgSetScroll(2, screenX, screenY);
    bgUpdate();
}

u8 *GraphicsSystem::getMobTiles() const {
    return (u8 *) mobTiles.data();
}

u8 *GraphicsSystem::getSubBgTiles() const {
    return (u8 *) subBgTiles.data();
}

u8 *GraphicsSystem::getBlockTiles() const {
    return (u8 *) blockTiles.data();
}

u8 *GraphicsSystem::getBlockPal() const {
    return (u8 *) blockPal.data();
}

uint16 *GraphicsSystem::getMainBgPtr() const {
    return mainBgPtr;
}

uint16 *GraphicsSystem::getSubBgPtr() const {
    return subBgPtr;
}

int GraphicsSystem::getSubBgID() const {
    return subBgID;
}

bool GraphicsSystem::loadTextureFile(const char *fileName) {
    if (!SHOULD_LOAD) {
        loadDefaultTexture();
        updateTexture();
    }
    std::string temp(fileName);
    temp = MINE_DS_FOLDER
           TEXTURE_FOLDER + temp;
    FILE *texFile = fopen(temp.c_str(), "rb");
    if (!texFile) {
        loadDefaultTexture();
        updateTexture();
        return strcmp(fileName, "default") == 0;
    }

    auto *blockTilesMem = new uint32[TILES_ARRAY_LEN];
    auto *blockPalMem = new uint16[PAL_ARRAY_LEN];
    if (fread(blockTilesMem, sizeof(uint32), TILES_ARRAY_LEN, texFile) != TILES_ARRAY_LEN ||
        fread(blockPalMem, sizeof(uint16), PAL_ARRAY_LEN, texFile) != PAL_ARRAY_LEN) {
        delete[] blockTilesMem;
        delete[] blockPalMem;
        blockTilesMem = nullptr;
        blockPalMem = nullptr;
    }

    auto *mobTilesMem = new uint32[TILES_ARRAY_LEN];
    auto *mobPalMem = new uint16[PAL_ARRAY_LEN];
    if (fread(mobTilesMem, sizeof(uint32), MOB_TILES_ARRAY_LEN, texFile) != MOB_TILES_ARRAY_LEN ||
        fread(mobPalMem, sizeof(uint16), MOB_PAL_ARRAY_LEN, texFile) != MOB_PAL_ARRAY_LEN) {
        delete[] mobTilesMem;
        delete[] mobPalMem;
        mobTilesMem = nullptr;
        mobPalMem = nullptr;
    }

    auto *subBgTilesMem = new uint32[TILES_ARRAY_LEN];
    auto *subBgPalMem = new uint16[PAL_ARRAY_LEN];
    if (fread(subBgTilesMem, sizeof(uint32), TILES_ARRAY_LEN, texFile) != TILES_ARRAY_LEN ||
        fread(subBgPalMem, sizeof(uint16), PAL_ARRAY_LEN, texFile) != PAL_ARRAY_LEN) {
        delete[] subBgTilesMem;
        delete[] subBgPalMem;
        subBgTilesMem = nullptr;
        subBgPalMem = nullptr;
    }

    fclose(texFile);

    loadTexture(blockTilesMem, blockPalMem, mobTilesMem, mobPalMem, subBgTilesMem, subBgPalMem);
    updateTexture();

    delete[] blockTilesMem;
    delete[] blockPalMem;
    delete[] mobTilesMem;
    delete[] mobPalMem;
    delete[] subBgTilesMem;
    delete[] subBgPalMem;
    return true;
}
