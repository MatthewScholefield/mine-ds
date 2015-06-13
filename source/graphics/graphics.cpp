#include <nds.h>
#include <stdio.h>
#include "graphics.h"
#include "mobs.h"
#include "particles.h"
#include "sub.h"
#include "sub_bg.h"
#include "block_small.h"
#include "font.h"
#include <vector>
#include <algorithm>
#include "../general.h"
#include "../worldRender.h"
#include "../files.h"

//A pointer to the loaded texture
unsigned int const *loadedTextureTiles;
unsigned short const *loadedTexturePalette;

//A pointer to the non-const texture data loaded into memory
unsigned int *loadedTilesMem = NULL;
unsigned short *loadedPalMem = NULL;

//A comment from 1995 :D

/**
	\file graphics.cpp
	\breif A file containing Sprite and particle related functions.
 */
//We need to have support for 8x8, 16x16, 64x64, 32x32 particles!
//We also need to have support for mob images.

/** \brief Main OAM Sprite ID counter.
	Don't modify values during runtime other than by using graphicNextMain() and graphicFrame().
 */



int MainSpr;
/** \brief Sub OAM Sprite ID counter.
 *	Don't modify values during runtime other than by using graphicNextSub() and graphicFrame().
 */
int SubSpr;

/**
	\breif Returns the next SpriteID for the Main OAM.
	\return The next SpriteID for the Main OAM.
 */
int graphicNextMain()
{
	return ++MainSpr;
}

/**
	\breif Returns the next SpriteID for the Sub OAM.
	\return The next SpriteID for the Sub OAM.
 */
int graphicNextSub()
{
	return ++SubSpr;
}

/**
	\breif Resets the SpriteID counters and clears the OAM.
	Should be called once per frame.
 */
void graphicFrame()
{
	MainSpr = -1;
	SubSpr = -1;
	oamClear(&oamMain, 0, 0);
	oamClear(&oamSub, 0, 0);
}

void setBlockPalette(int darkness, int index, bool isolated)
{
	darkness = 15 - darkness;
	if (isolated)
		vramSetBankF(VRAM_F_LCD);
	unsigned short *palette = new unsigned short[256];
	for (int i = 0; i < 256; ++i)
	{
		unsigned short slot = loadedTexturePalette[i];
		unsigned short blue = slot & 0x1F;
		slot >>= 5;
		unsigned short green = slot & 0x1F;
		slot >>= 5;
		unsigned short red = slot;
		blue = (blue * darkness) / 15;
		green = (green * darkness) / 15;
		red = (red * darkness) / 15; //*/
		slot = red;
		slot <<= 5;
		slot |= green;
		slot <<= 5;
		slot |= blue;
		palette[i] = slot;
	}
	dmaCopy(palette, VRAM_F_EXT_SPR_PALETTE[index], TEXTURE_PAL_LEN);
	delete[] palette;
	if (isolated)
		vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
}

void loadTexture(const unsigned int *sourceTilesMem, const unsigned short *sourcePalMem, bool clearMem)
{
	if (clearMem)
	{
		delete[] loadedTilesMem;
		delete[] loadedPalMem;
		loadedTilesMem = NULL;
		loadedPalMem = NULL;
	}
	loadedTextureTiles = sourceTilesMem;
	loadedTexturePalette = sourcePalMem;
}

void loadMemTex(unsigned int *sourceTilesMem, unsigned short *sourcePalMem)
{
	if (loadedTilesMem)
		delete[] loadedTilesMem;
	if (loadedPalMem)
		delete[] loadedPalMem;
	loadedTilesMem = sourceTilesMem;
	loadedPalMem = sourcePalMem;
	loadTexture(sourceTilesMem, sourcePalMem);
}

void updateTexture()
{
	vramSetBankE(VRAM_E_LCD);
	dmaCopy(loadedTexturePalette, VRAM_E_EXT_PALETTE[2][0], TEXTURE_PAL_LEN); //Copy the palette
	for (int i = 1; i < 16; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			uint16 col = VRAM_E_EXT_PALETTE[2][0][j];
			uint16 r = (col >> 0) & 0x1F;
			uint16 g = (col >> 5) & 0x1F;
			uint16 b = (col >> 10) & 0x1F;
			uint16 a = (col >> 15) & 0x1;
			int brightness = (16 - i)*16;
			r = (r * brightness) / 256;
			g = (g * brightness) / 256;
			b = (b * brightness) / 256;
			VRAM_E_EXT_PALETTE[2][i][j] = r << 0 |
					g << 5 |
					b << 10 |
					a << 15;
		}
	}
	for (int j = 0; j < 256; ++j)
		VRAM_E_EXT_PALETTE[2][15][j] = 0;
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);

	vramSetBankF(VRAM_F_LCD);
	for (int i = 3; i < 16; ++i)
		setBlockPalette((15 * (i - 3)) / 12, i, false);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);

	vramSetBankI(VRAM_I_LCD); //Loads Sub Screen Block Graphics
	dmaCopy(loadedTexturePalette, VRAM_I_EXT_SPR_PALETTE[2], TEXTURE_PAL_LEN);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	dmaCopy(loadedTextureTiles, bgGetGfxPtr(2), TEXTURE_TILES_LEN); //Copy Tiles
}

void loadDefaultTexture()
{
	//Load with arrays generated from GRIT
	loadTexture(block_smallTiles, block_smallPal, true);
}
/**
	\breif Init's the DS's Sprite Engine, to be used for MineDS.
	Should be called when initing the DS.
 */
void graphicsInit()
{
	loadTexture(getGlobalSettings()->textureName.c_str());
	updateTexture();
	worldRender_Init();
	setBackdropColor(RGB15(17, 24, 31));
	
	graphicFrame();
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

	//Start copying palettes
	vramSetBankF(VRAM_F_LCD);
	setBlockPalette(0, 2, false);
	dmaCopy(mobsPal, VRAM_F_EXT_SPR_PALETTE[0], mobsPalLen);
	dmaCopy(particlesPal, VRAM_F_EXT_SPR_PALETTE[1], particlesPalLen);
	for (int i = 3; i < 16; ++i)
		setBlockPalette((15 * (i - 3)) / 12, i, false);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
}

void loadGraphicMob(Graphic* g, int frame, int x, int y)
{
	if (y == 32)
	{
		u16* graphics = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
		u8* Tiles = (u8*) & mobsTiles;
		Tiles += frame * FRAMES_PER_ANIMATION * (16 * 32);
		dmaCopy(Tiles, graphics, 16 * 32);
		g->type = 1;
		g->paletteID = 0;
		g->sx = 16;
		g->sy = 32;
		g->Gfx = graphics;
	}
	else //Small mob
	{
		u16* graphics = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
		u8* Tiles = (u8*) & mobsTiles;
		Tiles += frame * FRAMES_PER_ANIMATION * (16 * 16);
		dmaCopy(Tiles, graphics, 16 * 16);
		g->type = 1;
		g->paletteID = 0;
		g->sx = 16;
		g->sy = 16;
		g->Gfx = graphics;
	}
}

void loadGraphicParticle(Graphic* g, int frame, int x, int y)
{
	if (x == 8 && y == 8)
	{
		u16 * graphics = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_256Color);
		u8* Tiles = (u8*) & particlesTiles;
		Tiles += frame * (8 * 8);
		dmaCopy(Tiles, graphics, 8 * 8);
		g->type = 0;
		g->paletteID = 1;
		g->sx = x;
		g->sy = y;
		g->Gfx = graphics;
	}
	else
		iprintf("Error loading graphics 1!\n");
}

void loadGraphicBlock(Graphic* g, int frame, int x, int y)
{
	u16 * graphics = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) loadedTextureTiles;
	Tiles += frame * (16 * 16);
	dmaCopy(Tiles, graphics, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 2, graphics + 8 * 4, 8 * 8);
	dmaCopy(Tiles + 8 * 8, graphics + 8 * 4 * 2, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 3, graphics + 8 * 4 * 3, 8 * 8);
	g->type = 2;
	g->paletteID = 2;
	g->sx = x;
	g->sy = y;
	g->Gfx = graphics;
}

void loadGraphicMiniBlock(Graphic* g, int frame, int x, int y, int paletteID)
{
	u16 * graphics = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_256Color);
	/*for (int i : loadedTexturePalette)*/
	u8* Tiles = ((u8*) loadedTextureTiles);
	Tiles += frame * (16 * 16);
	dmaCopy(Tiles + 8 * 8, graphics, 8 * 8);
	//dmaCopy(Tiles, graphics, 8 * 8);

	g->type = 4;
	g->paletteID = paletteID;
	g->sx = x;
	g->sy = y;
	g->Gfx = graphics;
	g->main = true;
}

void loadGraphicAnim(Graphic *sprite, u8* gfx, int frame)
{
	gfx += frame * FRAMES_PER_ANIMATION * (16 * 32);
	sprite->type = 3;
	sprite->paletteID = 0;
	sprite->sx = 16;
	sprite->sy = 32;
	sprite->Gfx = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
	sprite->frame_gfx = gfx;
}

void drawAnimFrame(Graphic* g, int mobSlot, int frame)
{
	int slot = frame + mobSlot * (FRAMES_PER_ANIMATION);

	u8* offset = g->frame_gfx + slot * 16 * 32;

	dmaCopy(offset, g->Gfx, 16 * 32);
}

void animateMob(Graphic* g, int mobSlot)
{
	++g->anim_frame;
	if (g->anim_frame >= FRAMES_PER_ANIMATION)
		g->anim_frame = 0;
	drawAnimFrame(g, mobSlot, g->anim_frame);
}

void setAnimFrame(Graphic* g, int mobSlot, int frame)
{
	g->anim_frame = frame;
	drawAnimFrame(g, mobSlot, g->anim_frame);
}

/**
	\breif A function used to load graphics for use on the Main screen, The screen with all of the blocks on it.
	\param g A pointer to a newly allocated Graphic structure.
	\param mob To choose between loading a mob image (true) or a 8x8 particle (false).
	\param frame Tile of Graphic to load
	\param x x Size of Tile
	\param y y Size of Tile
 */
void loadGraphic(Graphic* g, int mob, int frame, int x, int y)
{
	switch (mob)
	{
		case 0:
			loadGraphicParticle(g, frame, x, y);
			break;
		case 1:
			loadGraphicMob(g, frame, x, y);
			break;
		case 2:
			loadGraphicBlock(g, frame, x, y);
			break;
		case 3:
			loadGraphicAnim(g, (u8*) mobsTiles, 0);
			break;
	}
	g->main = true;
}

void loadGraphicAnim(Graphic* g, int frame)
{
	loadGraphicAnim(g, (u8*) mobsTiles, frame);
	g->main = true;
}

/**
	\breif A function used to load graphics for use on the Main screen, the screen with all the blocks on it.
	This function assmumes you are using a particle of 8x8 size or a Mob of 16x32 size
	\param g A pointer to a newly allocated Graphic structure.
	\param mob To choose between loading a mob image (true) or a 8x8 particle (false).
	\param frame Tile of Graphic to load
 */
void loadGraphic(Graphic* g, int mob, int frame)
{
	if (!mob) loadGraphic(g, mob, frame, 8, 8);
	else loadGraphic(g, mob, frame, 16, 32);
}

/**
	\breif A function used to free the graphics memory a graphic is using.
	\param g A pointer to a Graphic structure that has been loaded
 */
void unloadGraphic(Graphic* g)
{
	if (g->main)
		oamFreeGfx(&oamMain, g->Gfx);
	else
		oamFreeGfx(&oamSub, g->Gfx);
}

void loadGraphicSubParticle(Graphic* g, int frame, int x, int y)
{
	if (x == 8 && y == 8)
	{
		u16 * graphics = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
		u8* Tiles = (u8*) & subTiles;
		Tiles += frame * (8 * 8);
		dmaCopy(Tiles, graphics, 8 * 8);
		g->type = 0;
		g->paletteID = 0;
		g->sx = x;
		g->sy = y;
		g->Gfx = graphics;
	}
	else
		iprintf("Error loading graphics 2!\n");
}

void loadGraphicSubFont(Graphic* g, int frame, int x, int y)
{
	if (x == 8 && y == 8)
	{
		u16 * graphics = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
		u8* Tiles = (u8*) & fontTiles;
		Tiles += frame * (8 * 8);
		dmaCopy(Tiles, graphics, 8 * 8);
		g->type = 1;
		g->paletteID = 1;
		g->sx = x;
		g->sy = y;
		g->Gfx = graphics;
	}
	else
		iprintf("Error loading graphics 3!\n");
}

void loadGraphicSubBlock(Graphic* g, int frame, int x, int y)
{
	u16 * graphics = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) loadedTextureTiles;
	Tiles += frame * (16 * 16);
	dmaCopy(Tiles, graphics, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 2, graphics + 8 * 4, 8 * 8);
	dmaCopy(Tiles + 8 * 8, graphics + 8 * 4 * 2, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 3, graphics + 8 * 4 * 3, 8 * 8);
	g->type = 2;
	g->paletteID = 2;
	g->sx = x;
	g->sy = y;
	g->Gfx = graphics;
}

/**
	\breif A function used to load graphics for use on the Sub screen, The screen with the Logo on it.
	\param g A pointer to a newly allocated Graphic structure.
	\param font To choose between loading different types of graphic 0 = Particle 1 = font 2 = Block
	\param frame Tile of Graphic to load
	\param x x Size of Tile
	\param y y Size of Tile
 */
void loadGraphicSub(Graphic* g, int font, int frame, int x, int y)
{
	if (font == 1) loadGraphicSubFont(g, frame, x, y);
	else if (font == 2) loadGraphicSubBlock(g, frame, x, y);
	else if (!font) loadGraphicSubParticle(g, frame, x, y);
	g->main = false;
}

/**
	\breif A function used to show a Graphic file.
	\param g A pointer to a loaded Graphic structure.
	\param x The x position of where the sprite should be displayed.
	\param y The y position of where the sprite should be displayed.
 */
bool showGraphic(Graphic* g, int x, int y, bool flip, int pri)
{
	int spriteID = g->main ? graphicNextMain() : graphicNextSub();
	if (x<-32 || x > 256 + 32 || y<-32 || y > 244) return false;
	if (spriteID > SPRITE_COUNT - 1) return false;
	if (g->main)
	{
		switch (g->type)
		{
			case 0://1
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_8x8, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case 1://0
				if (g->sy == 32)
					oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x32, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				else
					oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x16, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case 2://2
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x16, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case 3://0
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x32, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case 4://miniBlock
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_8x8, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
		}
	}
	else
	{
		switch (g->type)
		{
			case 0:
				if (g->sx == 8 && g->sy == 8)
					oamSet(&oamSub, spriteID, x, y, pri, g->paletteID, SpriteSize_8x8, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case 1:
				oamSet(&oamSub, spriteID, x, y, pri, g->paletteID, SpriteSize_8x8, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case 2:
				oamSet(&oamSub, spriteID, x, y, pri, g->paletteID, SpriteSize_16x16, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
		}
	}
	return true;
}
