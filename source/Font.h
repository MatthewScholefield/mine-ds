#ifndef _FONT_H
#define _FONT_H

#include<nds.h>


struct fontStruct
{
	u32 fontHeader;
	u32 tileSize;
	u8 widths[256];
	u8 data[0];
};

class Font
{
	public:
	
	fontStruct* data;
	int tileLength;
	int tileSize;
	bool fromFile;
	
	Font();
	
	//Load from a file
	void loadFromFile(const char* file);
	
	//Load from a linked file in the data folder
	void load(const void* data);
	void unload();
};

extern Font tahoma11;
extern Font tahoma11bold;
void initFonts();

#endif