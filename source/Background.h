#ifndef _BACKGROUND_H
#define _BACKGROUND_H
#include<nds.h>
#include "Font.h"

void initBackgrounds();

class Background
{
	public:
	int id; //0..3 main 4..7 sub
	u16* gfx;
	
	Background();
	Background(int id, int offset);
	
	void setPixel(int x, int y, int val);
	int getPixel(int x, int y);
	void setScroll(int x, int y);
	void fill(int color);
	void fillRect(int color, int x0, int y0, int x1, int y1);
	void print(const char* text, Font& f, int color, int x, int y);
};


class TiledBackground
{
	public:
	int id; //0..3 main 4..7 sub
	u16* map;
	u16* gfx;
	TiledBackground();
	TiledBackground(int id, int offset, int tileOffset);
	
	void setTile(int x, int y, int val);
	int getTile(int x, int y);
	void setScroll(int x, int y);
	void clear();
};

extern Background bg2, bg3, bg6, bg7;
extern TiledBackground bg1;
#endif