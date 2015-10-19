#pragma once
#include <nds.h>
#define FRAMES_PER_ANIMATION 6

#define TILES_LEN 65536
#define PAL_LEN 512
#define MOB_TILES_LEN 30720
#define MOB_PAL_LEN 512

#define TILES_ARRAY_LEN TILES_LEN/4
#define PAL_ARRAY_LEN PAL_LEN/2
#define MOB_TILES_ARRAY_LEN MOB_TILES_LEN/4
#define MOB_PAL_ARRAY_LEN MOB_PAL_LEN/2

enum GraphicType
{
	GRAPHIC_PARTICLE = 0,
	GRAPHIC_MOB = 1,
	GRAPHIC_SUB_FONT = 1,
	GRAPHIC_BLOCK = 2,
	GRAPHIC_MOB_ANIM = 3,
	GRAPHIC_BLOCK_MINI = 4
};

/**
		\file graphics.h
		\breif A file used to define the Graphic structure.
 */
//! \breif Graphic Structure

typedef struct
{
	//! \breif Pointer to loaded graphic in VRAM
	u16* Gfx = nullptr;
	//! \breif Frame of animation
	u8* frameGfx = nullptr;
	//! \breif animation state
	int state = 0;
	//! \breif Animation frame
	int animFrame = 0;
	//! \breif x Size of Graphic in pixels.
	int sx = 0;
	//! \breif y Size of Graphic in pixels.
	int sy = 0;
	//! \breif Whether Graphic is a mob or a particle.
	GraphicType type = GRAPHIC_BLOCK;
	//! \breif Whether loaded for main or sub OAM.
	bool main = true;
	//! \breif The palette index to use
	int paletteID = 0;
	//! \breif The part of the image to crop
	int frame = 0;
	//! \breif The loaded texture ID. Used to trigger reload on texture change
	int loadIter = 0;
	bool drawn = false;
	bool ownsGfx = true;
} Graphic;

//Misc
void graphicsInit();
void clearMainGraphics();
void clearSubGraphics();
void unloadGraphic(Graphic* g);
void animateMob(Graphic* g, int mobSlot);
void setAnimFrame(Graphic* g, int mobSlot, int frame);
void setBlockPalette(bool block, int brightness, int index = 2);
bool showGraphic(Graphic* g, int x, int y, bool flip = false, int pri = 0);

//Loading
bool loadGraphic(Graphic* g, GraphicType type, int frame, int x, int y, int pID = 0);
bool loadGraphic(Graphic* g, GraphicType type, int frame);
void loadGraphicSub(Graphic* g, GraphicType type, int frame, int x = 8, int y = 8);

//Textures
void loadTexture(const unsigned int *blockTilesSrc, const unsigned short *blockPalSrc,
				 const unsigned int *mobTilesSrc, const unsigned short *mobPalSrc,
				 const unsigned int *subBgTilesSrc, const unsigned short *subBgPalSrc);
void updateTexture();
void loadDefaultTexture();
void setSkyColor(double, double, double, double, double, double);
void gradientHandler();

bool setCloneGraphic(Graphic *source, Graphic *clone);