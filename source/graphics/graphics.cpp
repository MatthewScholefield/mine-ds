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
#include "../world/worldRender.h"
#include "../files.h"
#include "../fontHandler.h"
#include "../dayNight.h"
#include "subBgHandler.h"

std::vector<unsigned int> blockTiles, mobTiles, subBgTiles;
std::vector<unsigned short> blockPal, mobPal, subBgPal;

int Graphic::textureID = 0, Graphic::nextSpriteIDMain = 0, Graphic::nextSpriteIDSub = 0;

uint16 backdropColor[192];
double gradientData[192][3];

bool mainSpriteUsed[128] = {};
bool subSpriteUsed[128] = {};

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

static void setBlockPalette(bool blocks, int brightness, int index)
{
	unsigned short *palette = new unsigned short[PAL_LEN / 2];
	for (int i = 0; i < PAL_LEN / 2; ++i)
	{
		unsigned short slot = blocks ? blockPal.data()[i] : mobPal.data()[i];
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
				 const unsigned int *subBgTilesSrc, const unsigned short *subBgPalSrc, bool skipReload)
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
	if (!skipReload)
		++Graphic::textureID;
}

void updateTexture()
{
	const int NUM_BANK_SLOTS = 16;
	const int MAX_BRIGHTNESS = 15;
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
	loadTexture(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, true);
}

void graphicsInit()
{
	loadTexture(getGlobalSettings()->textureName.c_str());
	worldRender_Init();
	resetSky();

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
}

int Graphic::nextSpriteID(bool main)
{
	if (main)
		return ++nextSpriteIDMain;
	else
		return ++nextSpriteIDSub;
}

OamState &Graphic::getOAM(bool main)
{
	return main ? oamMain : oamSub;
}

void Graphic::resetSprites(bool main)
{
	if (main)
		nextSpriteIDMain = 0;
	else
		nextSpriteIDSub = 0;
	oamClear(&getOAM(main), 0, 127);
}

void Graphic::loadSmallMob()
{
	Gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy((u8*) mobTiles.data() + frame * FRAMES_PER_ANIMATION * 16 * 16, Gfx, 16 * 16);
}

void Graphic::loadLargeMob()
{
	Gfx = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((u8*) mobTiles.data() + frame * FRAMES_PER_ANIMATION * 16 * 32, Gfx, 16 * 32);
}

void Graphic::loadParticle()
{
	Gfx = oamAllocateGfx(main ? &oamMain : &oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
	dmaCopy((u8*) & particlesTiles + frame * (8 * 8), Gfx, 8 * 8);
	paletteID = main ? 15 : 0;
}

void Graphic::loadBlock()
{
	Gfx = oamAllocateGfx(main ? &oamMain : &oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	u8* source = (u8*) blockTiles.data() + frame * (16 * 16);
	const int TILE_SIZE = 8 * 8;
	dmaCopy(source, Gfx, 8 * 8);
	dmaCopy(source + TILE_SIZE * 2, Gfx + 8 * 4, TILE_SIZE);
	dmaCopy(source + TILE_SIZE, Gfx + 8 * 4 * 2, TILE_SIZE);
	dmaCopy(source + TILE_SIZE * 3, Gfx + 8 * 4 * 3, TILE_SIZE);
	if (!main)
		paletteID = 2;
}

void Graphic::loadMiniBlock()
{
	Gfx = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_256Color);
	dmaCopy((u8*) blockTiles.data() + frame * (16 * 16) + 8 * 8, Gfx, 8 * 8);
}

void Graphic::loadAnim()
{
	frameGfx = (u8*) mobTiles.data() + frame * FRAMES_PER_ANIMATION * (16 * 32);
	Gfx = oamAllocateGfx(&oamMain, SpriteSize_16x32, SpriteColorFormat_256Color);
}

void Graphic::loadFrame()
{
	u8* newGfx = frameGfx + animFrame * 16 * 32;
	dmaCopy(newGfx, Gfx, 16 * 32);
}

void Graphic::animate()
{
	if (++animFrame >= FRAMES_PER_ANIMATION)
		animFrame = 0;
	loadFrame();
}

void Graphic::setFrame(int frame)
{
	animFrame = frame;
	loadFrame();
}

void Graphic::load()
{
	switch (type)
	{
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

void Graphic::reload(GraphicType type, int frame, bool main, int paletteID)
{
	this->Graphic::~Graphic();
	this->type = type;
	this->frame = frame;
	this->main = main;
	this->paletteID = paletteID;
	load();
}

void Graphic::reload()
{
	reload(type, frame, main, paletteID);
}

Graphic::Graphic(GraphicType type, int frame, bool main, int paletteID)
: Gfx(nullptr), frameGfx(nullptr), type(type), main(main), frame(frame)
, loadIter(textureID), paletteID(paletteID), animFrame(0)
{
	load();
}

Graphic::Graphic(const Graphic& orig) : Gfx(nullptr), frameGfx(nullptr), type(orig.type)
, main(orig.main), frame(orig.frame), loadIter(textureID), paletteID(orig.paletteID), animFrame(orig.animFrame)
{
	load();
}

Graphic::Graphic() : Gfx(nullptr), frameGfx(nullptr), type(GraphicType::NONE)
, main(true), frame(0), loadIter(0), paletteID(0), animFrame(0) { }

Graphic &Graphic::operator=(const Graphic &orig)
{
	reload(orig.type, orig.frame, orig.main, orig.paletteID);
	return *this;
}

SpriteSize Graphic::getSpriteSize(GraphicType type)
{
	switch (type)
	{
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

bool Graphic::draw(int x, int y, bool flip, int pri)
{
	if (!Gfx || x < -16 || x > 256 || y < -32 || y > 192)
		return false;
	if (loadIter != Graphic::textureID)
		reload();
	oamSet(&getOAM(main), nextSpriteID(main), x, y, pri, paletteID, getSpriteSize(type)
		, SpriteColorFormat_256Color, Gfx, -1, false, false, flip, false, false);
	return true;
}