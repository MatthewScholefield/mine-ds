#include <nds.h>
/** 
	\file graphics.h
	\breif A file used to define the Graphic structure.
*/
//! \breif Graphic Structure
typedef struct
{
	//! \breif Pointer to loaded graphic in VRAM
	u16* Gfx;
	//! \breif x Size of Graphic in pixels.
	int sx;
	//! \breif y Size of Graphic in pixels.
	int sy;
	//! \breif Whether Graphic is a mob or a particle.
	int mob;
	//! \breif Whether loaded for main or sub OAM.
	bool main;
}Graphic;
void graphicsInit(); 
void graphicFrame();
void loadGraphicMob(Graphic* g,int frame);
void loadGraphic(Graphic* g,int mob,int frame,int x,int y);
void loadGraphic(Graphic* g,int mob,int frame);
void loadGraphicSubMob(Graphic* g,int frame);
void loadGraphicSub(Graphic* g,int mob,int frame,int x,int y);
void loadGraphicSub(Graphic* g,int mob,int frame);
bool showGraphic(Graphic* g,int x,int y);
bool showGraphic(Graphic* g,int x,int y,bool flip);
void unloadGraphic(Graphic* g);
#define unloadGraphicSub(x) unloadGraphic(x)
