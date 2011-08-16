#include "Font.h"
#include<cstdio>

#include "tahoma11_bin.h"
#include "tahoma11bold_bin.h"

Font tahoma11;
Font tahoma11bold;

void initFonts()
{
	tahoma11.load(tahoma11_bin);
	tahoma11bold.load(tahoma11bold_bin);
}

Font::Font(){}

void Font::loadFromFile(const char* file)
{
	FILE *f = fopen(file, "rb");
	fseek(f, 0, SEEK_END);

	int size = ftell(f);

	fseek(f, 0, SEEK_SET);
	data = (fontStruct*) new u8[size+1];
	fread(data, 1, size, f);

	fclose(f);
	
	tileSize = data->tileSize;
	tileLength = tileSize*tileSize / 8;
	
	fromFile = true;
}


void Font::load(const void* data)
{
	this->data = (fontStruct*) data;
	tileSize = this->data->tileSize;
	tileLength = tileSize*tileSize / 8;
	fromFile = false;
}

void Font::unload()
{
	if(fromFile)
		delete data;
	data = NULL;
}