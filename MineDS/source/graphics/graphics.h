#include <nds.h>
#define FRAMES_PER_ANIMATION 6
/** 
	\file graphics.h
	\breif A file used to define the Graphic structure.
*/
//! \breif Graphic Structure
typedef struct
{
	//! \breif Pointer to loaded graphic in VRAM
	u16* Gfx;
        //Frame of animation
        u8*  frame_gfx;
        //animation state
        int state;
        //Animation frame
        int anim_frame;
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
void loadGraphicSub(Graphic* g, int font, int frame, int x = 8, int y = 8);
void unloadGraphic(Graphic* g);
void animateMob(Graphic* g, int mobSlot);
void loadGraphicAnim(Graphic* g, int frame);
void setAnimFrame(Graphic* g, int mobSlot, int frame);
bool showGraphic(Graphic* g, int x, int y, bool flip = false, int pri = 0);
#define unloadGraphicSub(x) unloadGraphic(x)
