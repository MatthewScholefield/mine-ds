#include <nds.h>
#include <font.h>
#include <block_small.h>
#include <sub_bg.h>
#include <mobs.h>
#include <particles.h>
#include <sub.h>
#include "GraphicsSystem.hpp"
#include "Graphic.hpp"
#include "../FileSystem.hpp"
#include "SubRenderer.hpp"
#include "TitleFontSystem.hpp"
#include <string>

using std::vector;

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

void GraphicsSystem::bind(SubRenderer &titleGraphics) {
    this->titleGraphics = &titleGraphics;
}

void GraphicsSystem::updateTexture() {
    const int NUM_BANK_SLOTS = 16;
    const int MAX_BRIGHTNESS = 15;
    const int NUM_PALETTE_COLORS = 256;
    const int BLOCK_PAL_BG = 2;
    //=== Main Block BG ===
    vramSetBankE(VRAM_E_LCD);
    dmaCopyVec(texture.block.pal, VRAM_E_EXT_PALETTE[BLOCK_PAL_BG][NUM_BANK_SLOTS - 1]); //Copy the palette
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
    dmaCopyVec(texture.block.pal, VRAM_I_EXT_SPR_PALETTE[2]);
    vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
    dmaCopyVec(texture.block.tiles, bgGetGfxPtr(mainBgID));

    //=== Sub BG ===
    vramSetBankH(VRAM_H_LCD);
    dmaCopyVec(texture.subBg.pal, VRAM_H_EXT_PALETTE[2][0]);
    vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
    dmaCopy(texture.subBg.tiles.data(), bgGetGfxPtr(subBgID), sub_bgTilesLen);
    if (titleGraphics) {
        titleGraphics->getTitleFont().refresh();
        titleGraphics->updateSubBG();
    }

    swiWaitForVBlank(); //Prevents sub screen flicker
}

void GraphicsSystem::setBlockPalette(bool blocks, int brightness, int index) {
    auto *palette = new unsigned short[Texture::palLen / 2];
    for (int i = 0; i < Texture::palLen / 2; ++i) {
        uint16 slot = blocks ? texture.block.pal[i] : texture.mob.pal[i];
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
    DC_FlushRange(palette, Texture::palLen);
    dmaCopy(palette, VRAM_F_EXT_SPR_PALETTE[index], Texture::palLen);
    delete[] palette;
}

void GraphicsSystem::beginRender(int screenX, int screenY) {
    bgSetScroll(2, screenX, screenY);
    bgUpdate();
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

bool GraphicsSystem::loadTextureFile(const std::string &fileName, bool reloadGfx) {
    if (!SHOULD_LOAD) {
        texture.loadDefault();
        updateTexture();
    }
    auto fullPath = std::string(MINE_DS_FOLDER) + TEXTURE_FOLDER + fileName;
    FILE *texFile = fopen(fullPath.c_str(), "rb");
    bool success;
    if (texFile) {
        texture.load(texFile);
        fclose(texFile);
        success = true;
    } else {
        texture.loadDefault();
        success = fileName == "default";
    }
    if (reloadGfx) {
        ++Graphic::textureID;
    }
    updateTexture();
    return success;
}

const Texture &GraphicsSystem::getTexture() const {
    return texture;
}

SkySystem GraphicsSystem::getSkySystem() {
    return sky;
}

