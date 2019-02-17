#pragma once

#include <nds.h>
#include "../utils.hpp"

#define FRAMES_PER_ANIMATION 6

enum class GraphicType {
    NONE,
    PARTICLE,
    MOB_SMALL,
    MOB_LARGE,
    MOB_ANIM,
    BLOCK,
    BLOCK_MINI
};

class Graphics;

class Graphic {
    u16 *gfx; //Pointer to loaded graphic in VRAM
    u8 *firstFrame; //Pointer to source of first frame of animation
    GraphicType type;
    bool main; //Whether loaded for main or sub OAM.
    int frame; //The part of the image to crop
    int loadIter; //Used to trigger reload on texture change
    const Graphics &graphics;

    static int nextSpriteID(bool main);
    static OamState &getOAM(bool main);
    void loadSmallMob();
    void loadLargeMob();
    void loadParticle();
    void loadBlock();
    void loadMiniBlock();
    void loadAnim();
    void loadFrame();
    void load();
    SpriteSize getSpriteSize(GraphicType type);

public:
    static int textureID, nextSpriteIDMain, nextSpriteIDSub;
    int paletteID, animFrame; //Animation frame

    static void resetSprites(bool main);
    void animate();
    void setFrame(int frame);
    void reload(GraphicType type, int frame, bool main = true, int paletteID = 0);
    void reload();
    bool draw(cVec2i &pos, bool flip = false, int pri = 0);
    Graphic(Graphics &graphics, GraphicType type, int frame, bool main = true, int paletteID = 0);
    Graphic(const Graphic &orig);
    explicit Graphic(Graphics &graphics);
    Graphic &operator=(const Graphic &orig);

    ~Graphic() {
        if (gfx) {
            oamFreeGfx(main ? &oamMain : &oamSub, gfx);
        }
    }
};
