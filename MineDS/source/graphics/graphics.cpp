#include <nds.h>
#include <stdio.h>
#include "graphics.h"
#include "mobs.h"
#include "particles.h"
#include "sub.h"
#include "sub_bg.h"
#include "block_small.h"
#include "font.h"
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
	MainSpr=-1;
	SubSpr=-1;
	oamClear(&oamMain,0,128);
	oamClear(&oamSub,0,128);
}
/**
	\breif Init's the DS's Sprite Engine, to be used for MineDS.
	Should be called when initing the DS.
*/
void graphicsInit()
{
	graphicFrame();
	vramSetBankD(VRAM_D_SUB_SPRITE);
	//Set the bank for our Graphics.
	oamInit(&oamSub,SpriteMapping_1D_256,true);
        vramSetBankI(VRAM_I_LCD);
	//Vram I is for Sub Sprite Palette!
	dmaCopy(subPal,VRAM_I_EXT_SPR_PALETTE[0],subPalLen);
	dmaCopy(fontPal,VRAM_I_EXT_SPR_PALETTE[1],fontPalLen);
	dmaCopy(block_smallPal,VRAM_I_EXT_SPR_PALETTE[2],block_smallPalLen);
        vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	oamInit(&oamMain,SpriteMapping_1D_256,true);
	vramSetBankF(VRAM_F_LCD);
	//Start copying palettes
	dmaCopy(mobsPal,VRAM_F_EXT_SPR_PALETTE[0],mobsPalLen);
	dmaCopy(particlesPal,VRAM_F_EXT_SPR_PALETTE[1],particlesPalLen);
	dmaCopy(block_smallPal,VRAM_F_EXT_SPR_PALETTE[2],block_smallPalLen);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
}

void loadGraphicMob(Graphic* g,int frame,int x,int y)
{
	u16* graphics=oamAllocateGfx(&oamMain,SpriteSize_16x32, SpriteColorFormat_256Color);
	u8* Tiles=(u8*)&mobsTiles;
	Tiles+=frame*(16*32);
	dmaCopy(Tiles,graphics,16*32);
	g->mob=true;
	g->sx=16;
	g->sy=32;
	g->Gfx=graphics;
}

void loadGraphicParticle(Graphic* g, int frame,int x,int y)
{
	if (x==8 && y==8)
	{
		u16 * graphics=oamAllocateGfx(&oamMain,SpriteSize_8x8, SpriteColorFormat_256Color);
		u8* Tiles=(u8*)&particlesTiles;
		Tiles+=frame*(8*8);
		dmaCopy(Tiles,graphics,8*8);
		g->mob=false;
		g->sx=x;
		g->sy=y;
		g->Gfx=graphics;
	}
	else
	{
		iprintf("Error loading graphics!\n");
	}
}
void loadGraphicBlock(Graphic* g, int frame,int x,int y)
{
	u16 * graphics=oamAllocateGfx(&oamMain,SpriteSize_16x16, SpriteColorFormat_256Color);
	u8* Tiles=(u8*)&block_smallTiles;
	Tiles+=frame*(16*16);
	dmaCopy(Tiles,graphics,8*8);
	dmaCopy(Tiles+8*8*2,graphics+8*4,8*8);
	dmaCopy(Tiles+8*8,graphics+8*4*2,8*8);
	dmaCopy(Tiles+8*8*3,graphics+8*4*3,8*8);
	g->mob=2;
	g->sx=x;
	g->sy=y;
	g->Gfx=graphics;
}
/** 
	\breif A function used to load graphics for use on the Main screen, The screen with all of the blocks on it.
	\param g A pointer to a newly allocated Graphic structure. 
	\param mob To choose between loading a mob image (true) or a 8x8 particle (false).
	\param frame Tile of Graphic to load
	\param x x Size of Tile
	\param y y Size of Tile
*/
void loadGraphic(Graphic* g,int mob,int frame,int x,int y)
{
	if (mob == 1) loadGraphicMob(g,frame,x,y);
	else if (mob == 0) loadGraphicParticle(g,frame,x,y);
	else if (mob == 2) loadGraphicBlock(g,frame,x,y);
//	if (mob) loadGraphicMob(g,frame,x,y);
//	else if (!mob) loadGraphicParticle(g,frame,x,y);
	g->main=true;
}
/** 
	\breif A function used to load graphics for use on the Main screen, the screen with all the blocks on it.
	This function assmumes you are using a particle of 8x8 size or a Mob of 16x32 size
	\param g A pointer to a newly allocated Graphic structure. 
	\param mob To choose between loading a mob image (true) or a 8x8 particle (false).
	\param frame Tile of Graphic to load
*/
void loadGraphic(Graphic* g,int mob,int frame)
{
	if (!mob) loadGraphic(g,mob,frame,8,8);
	if (mob) loadGraphic(g,mob,frame,16,32);
}
/**
	\breif A function used to free the graphics memory a graphic is using.
	\param g A pointer to a Graphic structure that has been loaded
*/
void unloadGraphic(Graphic* g)
{
	if(g->main)
	{
		oamFreeGfx(&oamMain,g->Gfx);
	}	
	else
	{
		oamFreeGfx(&oamSub,g->Gfx);
	}
}
void loadGraphicSubParticle(Graphic* g, int frame,int x,int y)
{
	if (x==8 && y==8)
	{
		u16 * graphics=oamAllocateGfx(&oamSub,SpriteSize_8x8, SpriteColorFormat_256Color);
		u8* Tiles=(u8*)&subTiles;
		Tiles+=frame*(8*8);
		dmaCopy(Tiles,graphics,8*8);
		g->mob=false;
		g->sx=x;
		g->sy=y;
		g->Gfx=graphics;
	}
	else
	{
		iprintf("Error loading graphics!\n");
	}
}
void loadGraphicSubFont(Graphic* g, int frame,int x,int y)
{
	if (x==8 && y==8)
	{
		u16 * graphics=oamAllocateGfx(&oamSub,SpriteSize_8x8, SpriteColorFormat_256Color);
		u8* Tiles=(u8*)&fontTiles;
		Tiles+=frame*(8*8);
		dmaCopy(Tiles,graphics,8*8);
		g->mob=1;
		g->sx=x;
		g->sy=y;
		g->Gfx=graphics;
	}
	else
	{
		iprintf("Error loading graphics!\n");
	}
}
void loadGraphicSubBlock(Graphic* g, int frame,int x,int y)
{
	u16 * graphics=oamAllocateGfx(&oamSub,SpriteSize_16x16, SpriteColorFormat_256Color);
	u8* Tiles=(u8*)&block_smallTiles;
	Tiles+=frame*(16*16);
	dmaCopy(Tiles,graphics,8*8);
	dmaCopy(Tiles+8*8*2,graphics+8*4,8*8);
	dmaCopy(Tiles+8*8,graphics+8*4*2,8*8);
	dmaCopy(Tiles+8*8*3,graphics+8*4*3,8*8);
	g->mob=2;
	g->sx=x;
	g->sy=y;
	g->Gfx=graphics;
}
/** 
	\breif A function used to load graphics for use on the Sub screen, The screen with the Logo on it.
	\param g A pointer to a newly allocated Graphic structure. 
	\param font To choose between loading different types of graphic 0 = Particle 1 = font 2 = Block
	\param frame Tile of Graphic to load
	\param x x Size of Tile
	\param y y Size of Tile
*/
void loadGraphicSub(Graphic* g,int font,int frame,int x,int y)
{
	if (font == 1) loadGraphicSubFont(g,frame,x,y);
	else if (font == 2) loadGraphicSubBlock(g,frame,x,y);
	else if (!font) loadGraphicSubParticle(g,frame,x,y);
	g->main=false;
}
/** 
	\breif A function used to load graphics for use on the Sub screen, The screen with the Logo on it.
	This function assmumes that the particle or font graphic is of 8x8 size.
	(Which is generally ok for the Sub screen)
	\param g A pointer to a newly allocated Graphic structure. 
	\param font To choose between loading different types of graphic 0 = Particle 1 = font 2 = Block
	\param frame Tile of Graphic to load
*/
void loadGraphicSub(Graphic* g,int font,int frame)
{
	loadGraphicSub(g,font,frame,8,8);
}
/**
	\breif A function used to show a Graphic file.
	\param g A pointer to a loaded Graphic structure.
	\param x The x position of where the sprite should be displayed.
	\param y The y position of where the sprite should be displayed.
*/
bool showGraphic(Graphic* g,int x,int y,bool flip)
{
	int spriteID;
	if (x<-32 || x>256+32 || y<-32 || y>244) return false;
	if (spriteID > 126) return false;
	if (g->main)
	{
		spriteID = graphicNextMain();
		if (g->mob==1)
			oamSet(&oamMain,spriteID,x,y,0,0,SpriteSize_16x32,SpriteColorFormat_256Color,g->Gfx,-1,false,false,flip,false,false); 
		else if (g->mob==0)
			oamSet(&oamMain,spriteID,x,y,0,1,SpriteSize_8x8,SpriteColorFormat_256Color,g->Gfx,-1,false,false,flip,false,false); 
		else if (g->mob==2)
			oamSet(&oamMain,spriteID,x,y,0,2,SpriteSize_16x16,SpriteColorFormat_256Color,g->Gfx,-1,false,false,flip,false,false); 
	}
	else
	{
		spriteID = graphicNextSub();
		if (g->mob==1)
		{
			oamSet(&oamSub,spriteID,x,y,0,1,SpriteSize_8x8,SpriteColorFormat_256Color,g->Gfx,-1,false,false,flip,false,false); 
		}
		else if (g->mob==2)
		{
			oamSet(&oamSub,spriteID,x,y,0,2,SpriteSize_16x16,SpriteColorFormat_256Color,g->Gfx,-1,false,false,flip,false,false); 
		}
		else if (g->sx==8 && g->sy==8 && g->mob==0)
			oamSet(&oamSub,spriteID,x,y,0,0,SpriteSize_8x8,SpriteColorFormat_256Color,g->Gfx,-1,false,false,flip,false,false); 
	}
	if (spriteID > 127) return false;
	return true;
}
bool showGraphic(Graphic* g,int x,int y)
{
	return showGraphic(g,x,y,false);
}
