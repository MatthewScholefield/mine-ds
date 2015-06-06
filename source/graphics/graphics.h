#pragma once
#include <nds.h>
#define FRAMES_PER_ANIMATION 6
#define TEXTURE_TILES_LEN 65536
#define TEXTURE_TILES_ARRAY_LEN 16384
#define TEXTURE_PAL_LEN 512
#define TEXTURE_PAL_ARRAY_LEN 256
/**
	\file graphics.h
	\breif A file used to define the Graphic structure.
 */
//! \breif Graphic Structure

typedef struct {
	//! \breif Pointer to loaded graphic in VRAM
	u16* Gfx;
	//! \breif Frame of animation
	u8* frame_gfx;
	//! \breif animation state
	int state;
	//! \breif Animation frame
	int anim_frame;
	//! \breif x Size of Graphic in pixels.
	int sx;
	//! \breif y Size of Graphic in pixels.
	int sy;
	//! \breif Whether Graphic is a mob or a particle.
	int type;
	//! \breif Whether loaded for main or sub OAM.
	bool main;
	//! \breif the palette index to use
	int paletteID;
} Graphic;
void graphicsInit();
void graphicFrame();
void setBlockPalette(int brightness, int index = 2, bool isolated = true);
void loadGraphicMob(Graphic* g, int frame);
void loadGraphicMiniBlock(Graphic* g, int frame, int x, int y, int paletteID);
void loadGraphicParticle(Graphic* g, int frame, int x, int y);
void loadGraphic(Graphic* g, int mob, int frame, int x, int y);
void loadGraphic(Graphic* g, int mob, int frame);
void loadGraphicSubMob(Graphic* g, int frame);
void loadGraphicSub(Graphic* g, int font, int frame, int x = 8, int y = 8);
void unloadGraphic(Graphic* g);
void animateMob(Graphic* g, int mobSlot);
void loadGraphicAnim(Graphic* g, int frame);
void setAnimFrame(Graphic* g, int mobSlot, int frame);
bool showGraphic(Graphic* g, int x, int y, bool flip = false, int pri = 0);
//void loadTexture(const unsigned int (*tiles)[TEXTURE_TILES_ARRAY_LEN], const unsigned short int (*palette)[TEXTURE_PAL_ARRAY_LEN], bool loadToVRAM = true);
#define unloadGraphicSub(x) unloadGraphic(x)