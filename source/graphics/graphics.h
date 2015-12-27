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

enum class GraphicType
{
	NONE,
	PARTICLE,
	MOB_SMALL,
	MOB_LARGE,
	MOB_ANIM,
	BLOCK,
	BLOCK_MINI
};

class Graphic
{
	u16* Gfx; //Pointer to loaded graphic in VRAM
	u8* frameGfx; //Pointer to source of first frame of animation
	int animFrame; //Animation frame
	GraphicType type;
	bool main; //Whether loaded for main or sub OAM.
	int frame; //The part of the image to crop
	int loadIter; //Used to trigger reload on texture change

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
	SpriteSize Graphic::getSpriteSize(GraphicType type);

public:
	static int textureID, nextSpriteIDMain, nextSpriteIDSub;
	int paletteID;

	static void resetSprites(bool main);
	void animate();
	void setFrame(int frame);
	void reload(GraphicType type, int frame, bool main = true, int paletteID = 0);
	void reload();
	bool draw(int x, int y, bool flip, int pri);

	Graphic(GraphicType type, int frame, bool main = true, int paletteID = 0);
	Graphic(const Graphic &orig);
	Graphic();

	Graphic &operator=(const Graphic &orig);

	~Graphic()
	{
		if (Gfx)
			oamFreeGfx(main ? &oamMain : &oamSub, Gfx);
	}
};

//Misc
void graphicsInit();

//Textures
void loadTexture(const unsigned int *blockTilesSrc, const unsigned short *blockPalSrc,
				 const unsigned int *mobTilesSrc, const unsigned short *mobPalSrc,
				 const unsigned int *subBgTilesSrc, const unsigned short *subBgPalSrc, bool skipReload = false);
void updateTexture();
void loadDefaultTexture();
void setSkyColor(double, double, double, double, double, double);
void gradientHandler();