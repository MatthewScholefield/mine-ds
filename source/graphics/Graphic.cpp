#include <nds.h>
#include "Graphic.hpp"
#include "../utils.hpp"
#include "../FileSystem.hpp"
#include <vector>
#include <sub_bg.h>
#include <sub.h>
#include <particles.h>
#include <font.h>
#include <block_small.h>
#include <mobs.h>
#include "GraphicsSystem.hpp"

int Graphic::textureID = 0, Graphic::nextSpriteIDMain = 0, Graphic::nextSpriteIDSub = 0;


int Graphic::nextSpriteID(bool main) {
    if (main) {
        return ++nextSpriteIDMain;
    } else
        return ++nextSpriteIDSub;
}

OamState &Graphic::getOAM(bool main) {
    return main ? oamMain : oamSub;
}

void Graphic::resetSprites(bool main) {
    if (main) {
        nextSpriteIDMain = 0;
    } else
        nextSpriteIDSub = 0;
    oamClear(&getOAM(main), 0, 127);
}

void Graphic::loadSmallMob() {
    gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    dmaCopy((u8 *) graphics.getTexture().mob.tiles.data() + frame * FRAMES_PER_ANIMATION * 16 * 16, gfx, 16 * 16);
}

void Graphic::loadLargeMob() {
    gfx = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
    dmaCopy((u8 *) graphics.getTexture().mob.tiles.data() + frame * FRAMES_PER_ANIMATION * 16 * 32, gfx, 16 * 32);
}

void Graphic::loadParticle() {
    gfx = oamAllocateGfx(main ? &oamMain : &oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
    dmaCopy((u8 *) (main ? particlesTiles : subTiles) + frame * (8 * 8), gfx, 8 * 8);
    paletteID = main ? 15 : 0;
}

void Graphic::loadBlock() {
    gfx = oamAllocateGfx(main ? &oamMain : &oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    u8 *source = (u8 *) graphics.getTexture().block.tiles.data() + frame * (16 * 16);
    const int TILE_SIZE = 8 * 8;
    dmaCopy(source, gfx, 8 * 8);
    dmaCopy(source + TILE_SIZE * 2, gfx + 8 * 4, TILE_SIZE);
    dmaCopy(source + TILE_SIZE, gfx + 8 * 4 * 2, TILE_SIZE);
    dmaCopy(source + TILE_SIZE * 3, gfx + 8 * 4 * 3, TILE_SIZE);
    if (!main) {
        paletteID = 2;
    }
}

void Graphic::loadMiniBlock() {
    gfx = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_256Color);
    dmaCopy((u8 *) graphics.getTexture().block.tiles.data() + frame * (16 * 16) + 8 * 8, gfx, 8 * 8);
}

void Graphic::loadAnim() {
    firstFrame = (u8 *) graphics.getTexture().mob.tiles.data() + frame * FRAMES_PER_ANIMATION * (16 * 32);
    gfx = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
}

void Graphic::loadFrame() {
    u8 *newGfx = firstFrame + animFrame * 16 * 32;
    dmaCopy(newGfx, gfx, 16 * 32);
}

void Graphic::animate() {
    if (++animFrame >= FRAMES_PER_ANIMATION) {
        animFrame = 0;
    }
    loadFrame();
}

void Graphic::setFrame(int frame) {
    animFrame = frame;
    loadFrame();
}

void Graphic::load() {
    switch (type) {
        case GraphicType::NONE:
            return;
        case GraphicType::PARTICLE:
            loadParticle();
            break;
        case GraphicType::MOB_SMALL:
            loadSmallMob();
            break;
        case GraphicType::MOB_LARGE:
            loadLargeMob();
            break;
        case GraphicType::BLOCK:
            loadBlock();
            break;
        case GraphicType::MOB_ANIM:
            loadAnim();
            break;
        case GraphicType::BLOCK_MINI:
            loadMiniBlock();
            break;
    }
}

void Graphic::reload(GraphicType type, int frame, bool main, int paletteID) {
    this->Graphic::~Graphic();
    this->type = type;
    this->frame = frame;
    this->main = main;
    this->paletteID = paletteID;
    load();
}

void Graphic::reload() {
    reload(type, frame, main, paletteID);
}

Graphic::Graphic(GraphicsSystem &graphics, GraphicType type, int frame, bool main, int paletteID)
        : gfx(nullptr), firstFrame(nullptr), type(type), main(main), frame(frame), loadIter(textureID),
          graphics(graphics), paletteID(paletteID), animFrame(0) {}

Graphic::Graphic(const Graphic &orig) : gfx(nullptr), firstFrame(nullptr), type(orig.type), main(orig.main),
                                        frame(orig.frame), loadIter(textureID), graphics(orig.graphics),
                                        paletteID(orig.paletteID), animFrame(orig.animFrame) {
    load();
}

Graphic::Graphic(GraphicsSystem &graphics)
        : gfx(nullptr), firstFrame(nullptr), type(GraphicType::NONE), main(true),
          frame(0), loadIter(0), graphics(graphics), paletteID(0), animFrame(0) {}

Graphic &Graphic::operator=(const Graphic &orig) {
    reload(orig.type, orig.frame, orig.main, orig.paletteID);
    return *this;
}

SpriteSize Graphic::getSpriteSize(GraphicType type) {
    switch (type) {
        case GraphicType::PARTICLE:
            return SpriteSize_8x8;
        case GraphicType::MOB_LARGE:
            return SpriteSize_16x32;
        case GraphicType::MOB_SMALL:
            return SpriteSize_16x16;
        case GraphicType::BLOCK:
            return SpriteSize_16x16;
        case GraphicType::MOB_ANIM:
            return SpriteSize_16x32;
        case GraphicType::BLOCK_MINI:
            return SpriteSize_8x8;
        default:
            return SpriteSize_16x16;
    }
}

bool Graphic::draw(int x, int y, bool flip, int pri) {
    if (type == GraphicType::NONE) {
        return false;
    }
    if (!gfx) {
        load();
    }
    if (x < -16 || x > 256 || y < -32 || y > 192) {
        return false;
    }
    if (loadIter != Graphic::textureID) {
        reload();
    }
    oamSet(&getOAM(main), nextSpriteID(main), x, y, pri, paletteID, getSpriteSize(type), SpriteColorFormat_256Color,
           gfx, -1, false, false, flip, false, false);
    return true;
}
