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
#include "../fontHandler.h"
#include "../dayNight.h"
#include "subBgHandler.h"

std::vector<unsigned int> blockTiles, mobTiles, subBgTiles;
std::vector<unsigned short> blockPal, mobPal, subBgPal;
int textureID = 0;
uint16 backdropColor[192];
double gradientData[192][3];

bool mainSpriteUsed[128] = {};
bool subSpriteUsed[128] = {};

//A comment from 1995 :D

/**
	\file graphics.cpp
	\breif A file containing Sprite and particle related functions.
 */
//We need to have support for 8x8, 16x16, 64x64, 32x32 particles!
//We also need to have support for mob images.

/**
	\breif Returns the next SpriteID for the Main OAM.
	\return The next SpriteID for the Main OAM.
 */

int nextSpriteIDMain;
int nextSpriteIDSub;
static int graphicNextMain()
{
	return nextSpriteIDMain++;
}

/**
	\breif Returns the next SpriteID for the Sub OAM.
	\return The next SpriteID for the Sub OAM.
 */
static int graphicNextSub()
{
	return nextSpriteIDSub++;
}

void gradientHandler()
{
	int r = REG_VCOUNT + 1;
	if (r < 0) r = 191;
	if (r >= 192) r = 0;
	BG_PALETTE[0] = backdropColor[r];
}

void setSkyColor(double red1, double green1, double blue1, double red2, double green2, double blue2)
{
	if (getGlobalSettings()->getProperty(PROPERTY_GRADIENT))
	{
		for (double i = 0; i < 192; ++i)
		{
			gradientData[int(i)][0] = std::min(red1 * (1.0 - i / 191.0) + (red2 * i) / 191, 31.0);
			gradientData[int(i)][1] = std::min(green1 * (1.0 - i / 191.0) + (green2 * i) / 191, 31.0);
			gradientData[int(i)][2] = std::min(blue1 * (1.0 - i / 191.0) + (blue2 * i) / 191, 31.0);
		}
		if (getGlobalSettings()->getProperty(PROPERTY_DITHERING))
		{
			for (u16 i = 0; i < 191; ++i)
				for (u8 j = 0; j < 3; ++j)
				{
					double extra = gradientData[i][j] - ((int) gradientData[i][j]);
					gradientData[i][j] = int(gradientData[i][j]);
					gradientData[i + 1][j] += extra;
				}
			for (u8 i = 0; i < 3; ++i)
				gradientData[191][i] = int(gradientData[191][i]);
		}
		for (u16 i = 0; i < 192; ++i)
			backdropColor[i] = RGB15(int(gradientData[i][0]), int(gradientData[i][1]), int(gradientData[i][2]));
	}
	else
		for (u16 i = 0; i < 192; ++i)
			backdropColor[i] = RGB15(int((red1 + red2) / 2.0 + 0.5), int((green1 + green2) / 2.0 + 0.5), int((blue1 + blue2) / 2.0 + 0.5));
}

/**
	\breif Resets the SpriteID counters and clears the OAM.
	Should be called once per frame.
 */
void clearMainGraphics()
{
	nextSpriteIDMain = 0;
	oamClear(&oamMain,0,127);
}

void clearSubGraphics()
{
	nextSpriteIDSub = 0;
	oamClear(&oamSub,0,127);
}

void setBlockPalette(bool blocks, int brightness, int index)
{
	unsigned short *palette = new unsigned short[PAL_LEN/2];
	for (int i = 0; i < PAL_LEN/2; ++i)
	{
		unsigned short slot = blocks?blockPal.data()[i]:mobPal.data()[i];
		unsigned short blue = slot & 0x1F;
		slot >>= 5;
		unsigned short green = slot & 0x1F;
		slot >>= 5;
		unsigned short red = slot;
		blue = (blue * brightness) / 15;
		green = (green * brightness) / 15;
		red = (red * brightness) / 15; //*/
		slot = red;
		slot <<= 5;
		slot |= green;
		slot <<= 5;
		slot |= blue;
		palette[i] = slot;
	}
	dmaCopy(palette, VRAM_F_EXT_SPR_PALETTE[index], PAL_LEN);
	delete[] palette;
}

void loadTexture(const unsigned int *blockTilesSrc, const unsigned short *blockPalSrc,
				 const unsigned int *mobTilesSrc, const unsigned short *mobPalSrc,
				 const unsigned int *subBgTilesSrc, const unsigned short *subBgPalSrc)
{
	if (!blockTilesSrc || !blockPalSrc)
	{
		blockTiles.assign(block_smallTiles, block_smallTiles + TILES_ARRAY_LEN);
		blockPal.assign(block_smallPal, block_smallPal + PAL_ARRAY_LEN);
	}
	else
	{
		blockTiles.assign(blockTilesSrc, blockTilesSrc + TILES_ARRAY_LEN);
		blockPal.assign(blockPalSrc, blockPalSrc + PAL_ARRAY_LEN);
	}

	if (!subBgTilesSrc || !subBgPalSrc)
	{
		subBgTiles.assign(sub_bgTiles, sub_bgTiles + TILES_ARRAY_LEN);
		subBgPal.assign(sub_bgPal, sub_bgPal + PAL_ARRAY_LEN);
	}
	else
	{
		subBgTiles.assign(subBgTilesSrc, subBgTilesSrc + TILES_ARRAY_LEN);
		subBgPal.assign(subBgPalSrc, subBgPalSrc + PAL_ARRAY_LEN);
	}

	if (!mobTilesSrc || !mobPalSrc)
	{
		mobTiles.assign(mobsTiles, mobsTiles + MOB_TILES_ARRAY_LEN);
		mobPal.assign(mobsPal, mobsPal + MOB_PAL_ARRAY_LEN);
	}
	else
	{
		mobTiles.assign(mobTilesSrc, mobTilesSrc + MOB_TILES_ARRAY_LEN);
		mobPal.assign(mobPalSrc, mobPalSrc + MOB_PAL_ARRAY_LEN);
	}
	++textureID;
}

void updateTexture()
{
	const int NUM_BANK_SLOTS = 16;
	const int MAX_BRIGHTNESS = 16;
	const int NUM_PALETTE_COLORS = 256;
	const int BLOCK_PAL_BG = 2;
	//=== Main Block BG ===
	vramSetBankE(VRAM_E_LCD);
	dmaCopy(blockPal.data(), VRAM_E_EXT_PALETTE[BLOCK_PAL_BG][NUM_BANK_SLOTS - 1], PAL_LEN); //Copy the palette
	for (int i = 0; i < NUM_BANK_SLOTS; ++i)
	{
		for (int j = 0; j < NUM_PALETTE_COLORS; ++j)
		{
			uint16 col = VRAM_E_EXT_PALETTE[BLOCK_PAL_BG][NUM_BANK_SLOTS - 1][j];
			uint16 r = (col >> 0) & 0x1F;
			uint16 g = (col >> 5) & 0x1F;
			uint16 b = (col >> 10) & 0x1F;
			uint16 a = (col >> 15) & 0x1;
			r = (r * i) / (NUM_BANK_SLOTS - 1);
			g = (g * i) / (NUM_BANK_SLOTS - 1);
			b = (b * i) / (NUM_BANK_SLOTS - 1);
			VRAM_E_EXT_PALETTE[2][i][j] = r << 0 |
					g << 5 |
					b << 10 |
					a << 15;
		}
	}
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);

	
	vramSetBankF(VRAM_F_LCD);
	for (int i = 1; i <= NUM_BANK_SLOTS / 2; ++i)
	{
		//=== Main Block Gfx ===
		setBlockPalette(true, (MAX_BRIGHTNESS * i) / (NUM_BANK_SLOTS / 2), i - 1);
		//=== Mob Sprite Gfx ===
		setBlockPalette(false, (MAX_BRIGHTNESS * i) / (NUM_BANK_SLOTS / 2), i - 1 + NUM_BANK_SLOTS / 2);
	}
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);

	//=== Sub Block ===
	vramSetBankI(VRAM_I_LCD);
	dmaCopy(blockPal.data(), VRAM_I_EXT_SPR_PALETTE[2], PAL_LEN);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	dmaCopy(blockTiles.data(), bgGetGfxPtr(2), TILES_LEN);

	//=== Sub BG ===
	vramSetBankH(VRAM_H_LCD);
	dmaCopy(subBgPal.data(), VRAM_H_EXT_PALETTE[2][0], PAL_LEN);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	dmaCopy(subBgTiles.data(), bgGetGfxPtr(getSubBgID()), sub_bgTilesLen);
	refreshFont();
	updateSubBG();

	swiWaitForVBlank(); //Prevents sub screen flicker
}

void loadDefaultTexture()
{
	loadTexture(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

/**
	\breif Init's the DS's Sprite Engine, to be used for MineDS.
	Should be called when initing the DS.
 */
void graphicsInit()
{
	loadTexture(getGlobalSettings()->textureName.c_str());
	worldRender_Init();
	resetSky();

	clearMainGraphics();
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
	dmaCopy(particlesPal, VRAM_F_EXT_SPR_PALETTE[15], particlesPalLen);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
}

void loadGraphicMob(Graphic* g, int frame, int x, int y, int pID)
{
	if (y == 32)
	{
		u16* graphics = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
		u8* Tiles = (u8*) mobTiles.data();
		Tiles += frame * FRAMES_PER_ANIMATION * (16 * 32);
		dmaCopy(Tiles, graphics, 16 * 32);
		g->paletteID = pID;
		g->Gfx = graphics;
	}
	else //Small mob
	{
		u16* graphics = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
		u8* Tiles = (u8*) mobTiles.data();
		Tiles += frame * FRAMES_PER_ANIMATION * (16 * 16);
		dmaCopy(Tiles, graphics, 16 * 16);
		g->paletteID = pID;
		g->Gfx = graphics;
	}
}

void loadGraphicParticle(Graphic* g, int frame, int x, int y)
{
	u16 * graphics = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) & particlesTiles;
	Tiles += frame * (8 * 8);
	dmaCopy(Tiles, graphics, 8 * 8);
	g->paletteID = 3;
	g->Gfx = graphics;
}

void loadGraphicBlock(Graphic* g, int frame, int x, int y, int paletteID)
{
	u16 * graphics = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) blockTiles.data();
	Tiles += frame * (16 * 16);
	dmaCopy(Tiles, graphics, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 2, graphics + 8 * 4, 8 * 8);
	dmaCopy(Tiles + 8 * 8, graphics + 8 * 4 * 2, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 3, graphics + 8 * 4 * 3, 8 * 8);
	g->paletteID = paletteID;
	g->Gfx = graphics;
}

void loadGraphicMiniBlock(Graphic* g, int frame, int x, int y, int paletteID)
{
	u16 * graphics = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) blockTiles.data();
	Tiles += frame * (16 * 16);
	dmaCopy(Tiles + 8 * 8, graphics, 8 * 8);
	g->paletteID = paletteID;
	g->Gfx = graphics;
	g->main = true;
}

void loadGraphicAnim(Graphic *sprite, u8* gfx, int frame, int pID)
{
	gfx += frame * FRAMES_PER_ANIMATION * (16 * 32);
	sprite->paletteID = pID;
	sprite->sx = 16;
	sprite->sy = 32;
	sprite->Gfx = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
	sprite->frameGfx = gfx;
}

void drawAnimFrame(Graphic* g, int mobSlot, int frame)
{
	int slot = frame + mobSlot * (FRAMES_PER_ANIMATION);

	u8* offset = g->frameGfx + slot * 16 * 32;

	dmaCopy(offset, g->Gfx, 16 * 32);
}

void animateMob(Graphic* g, int mobSlot)
{
	++g->animFrame;
	if (g->animFrame >= FRAMES_PER_ANIMATION)
		g->animFrame = 0;
	drawAnimFrame(g, mobSlot, g->animFrame);
}

void setAnimFrame(Graphic* g, int mobSlot, int frame)
{
	g->animFrame = frame;
	drawAnimFrame(g, mobSlot, g->animFrame);
}

/**
	\breif A function used to load graphics for use on the Main screen, The screen with all of the blocks on it.
	\param g A pointer to a newly allocated Graphic structure.
	\param mob To choose between loading a mob image (true) or a 8x8 particle (false).
	\param frame Tile of Graphic to load
	\param x x Size of Tile
	\param y y Size of Tile
 */
bool loadGraphic(Graphic* g, GraphicType type, int frame, int x, int y, int pID)
{
	switch (type)
	{
	case GRAPHIC_PARTICLE:
		loadGraphicParticle(g, frame, x, y);
		break;
	case GRAPHIC_MOB:
		loadGraphicMob(g, frame, x, y, pID);
		break;
	case GRAPHIC_BLOCK:
		loadGraphicBlock(g, frame, x, y, pID);
		break;
	case GRAPHIC_MOB_ANIM:
		loadGraphicAnim(g, (u8*) mobTiles.data(), frame, pID);
		break;
	case GRAPHIC_BLOCK_MINI:
		loadGraphicMiniBlock(g, frame, x, y, pID);
	}
	g->type = type;
	g->main = true;
	g->frame = frame;
	g->loadIter = textureID;
	g->sx = x;
	g->sy = y;
	return true;
}

/**
	\breif A function used to load graphics for use on the Main screen, the screen with all the blocks on it.
	This function assmumes you are using a particle of 8x8 size or a Mob of 16x32 size
	\param g A pointer to a newly allocated Graphic structure.
	\param mob To choose between loading a mob image (true) or a 8x8 particle (false).
	\param frame Tile of Graphic to load
 */
bool loadGraphic(Graphic* g, GraphicType type, int frame)
{
	if (type == GRAPHIC_PARTICLE) return loadGraphic(g, type, frame, 8, 8);
	else return loadGraphic(g, type, frame, 16, 32);
}

/**
	\breif A function used to free the graphics memory a graphic is using.
	\param g A pointer to a Graphic structure that has been loaded
 */
void unloadGraphic(Graphic* g)
{
	if (!g->Gfx)
		return;
	if (g->main)
	{
		if (g->ownsGfx)
			oamFreeGfx(&oamMain, g->Gfx);
	}
	else
	{
		if (g->ownsGfx)
			oamFreeGfx(&oamSub, g->Gfx);
	}
}

void loadGraphicSubParticle(Graphic* g, int frame, int x, int y)
{
	u16 * graphics = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) & subTiles;
	Tiles += frame * (8 * 8);
	dmaCopy(Tiles, graphics, 8 * 8);
	g->paletteID = 0;
	g->Gfx = graphics;
}

void loadGraphicSubFont(Graphic* g, int frame, int x, int y)
{
	u16 * graphics = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) & fontTiles;
	Tiles += frame * (8 * 8);
	dmaCopy(Tiles, graphics, 8 * 8);
	g->paletteID = 1;
	g->Gfx = graphics;
}

void loadGraphicSubBlock(Graphic* g, int frame, int x, int y)
{
	u16 * graphics = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	u8* Tiles = (u8*) blockTiles.data();
	Tiles += frame * (16 * 16);
	dmaCopy(Tiles, graphics, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 2, graphics + 8 * 4, 8 * 8);
	dmaCopy(Tiles + 8 * 8, graphics + 8 * 4 * 2, 8 * 8);
	dmaCopy(Tiles + 8 * 8 * 3, graphics + 8 * 4 * 3, 8 * 8);
	g->paletteID = 2;
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
void loadGraphicSub(Graphic* g, GraphicType type, int frame, int x, int y)
{
	switch (type)
	{
	case GRAPHIC_SUB_FONT:
		loadGraphicSubFont(g, frame, x, y);
		break;
	case GRAPHIC_BLOCK:
		loadGraphicSubBlock(g, frame, x, y);
		break;
	case GRAPHIC_PARTICLE:
		loadGraphicSubParticle(g, frame, x, y);
		break;
	default:
		break;
	}
	g->main = false;
	g->frame = frame;
	g->loadIter = textureID;
	g->sx = x;
	g->sy = y;
	g->type = type;
}

/**
	\breif A function used to show a Graphic file.
	\param g A pointer to a loaded Graphic structure.
	\param x The x position of where the sprite should be displayed.
	\param y The y position of where the sprite should be displayed.
 */
bool showGraphic(Graphic* g, int x, int y, bool flip, int pri)
{
	if (x<-g->sx || x > 256 || y<-g->sy || y > 192)
		return false;
	if (!g->Gfx)
		return false;
	if (g->loadIter != textureID) //Must reload texture
	{
		unloadGraphic(g);
		if (g->main)
			loadGraphic(g, g->type, g->frame, g->sx, g->sy, g->paletteID);
		else
			loadGraphicSub(g, g->type, g->frame, g->sx, g->sy);
	}
	if (!g->drawn)
	{
		if (g->main)
		{
			int spriteID = graphicNextMain();
			switch (g->type)
			{
			case GRAPHIC_PARTICLE://1
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_8x8, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case GRAPHIC_MOB://0
				if (g->sy == 32)
					oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x32, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				else
					oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x16, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case GRAPHIC_BLOCK://2
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x16, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case GRAPHIC_MOB_ANIM://0
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_16x32, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			case GRAPHIC_BLOCK_MINI://miniBlock
				oamSet(&oamMain, spriteID, x, y, pri, g->paletteID, SpriteSize_8x8, SpriteColorFormat_256Color, g->Gfx, -1, false, false, flip, false, false);
				break;
			}
		}
		else
		{
			int spriteID = graphicNextSub();
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
			default:
				break;
			}
		}
	}
	return true;
	
}

bool setCloneGraphic(Graphic *source, Graphic *clone)
{
	clone->Gfx = source->Gfx;
	clone->frameGfx = source->frameGfx;
	clone->state = source->state;
	clone->animFrame = source->animFrame;
	clone->sx = source->sx;
	clone->sy = source->sy;
	clone->type = source->type;
	clone->main = source->main;
	clone->paletteID = source->paletteID;
	clone->frame = source->frame;
	clone->loadIter = source->loadIter;
	clone->drawn = source->drawn;
	clone->ownsGfx = false;
	return true;
}