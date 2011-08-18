#include "Background.h"
#include<nds.h>

void copy16(const void* src, void* dest, u32 len)
{
	len /= 2;
	for(u32 i = 0; i < len; i++)
		((u16*)dest)[i] = ((u16*)src)[i];
}

void initBackgrounds()
{
	//This is the background layout I use in my mp3 player.
	//You probably want to change it. ~Dirbaio
/*
	bg2 = Background (2, 4);
	bgSetPriority(2, 1);
	bg3 = Background (3, 8);

	bg1 = TiledBackground(1, 12, 0);
	bgSetPriority(1, 2);
	copy16(tilesTiles, bg1.gfx, tilesTilesLen);
	vramSetBankF(VRAM_F_LCD);
	copy16(tilesPal, VRAM_F_EXT_PALETTE[1], tilesPalLen);
	vramSetBankF(VRAM_F_BG_EXT_PALETTE_SLOT23);
	
	BG_PALETTE[0] = RGB15(0, 0, 0);
	BG_PALETTE[1] = RGB15(0, 0, 0);
	BG_PALETTE[2] = RGB15(31, 31, 31);*/
//	bg6 = Background (6, 0);
//	bg7 = Background (7, 4);
}

Background::Background(){}
Background::Background(int id, int offset)
{
	this->id = id;
	if(id < 4)
		bgInit(id, BgType_Bmp8, BgSize_B8_256x256, offset, 0);
	else
		bgInitSub(id-4, BgType_Bmp8, BgSize_B8_256x256, offset, 0);
		
	gfx = bgGetGfxPtr(id);
	fill(0);
	bgWrapOn(id);
	bgShow(id);
}

void Background::setPixel(int x, int y, int val)
{
	x &= 0xFF;
	y &= 0xFF;
	int pos = y*256+x;
	u16 g = gfx[pos/2];
	if(pos % 2 == 0)
		g = g & 0xFF00 | val;
	else
		g = g & 0x00FF | val<<8;
		
	gfx[pos/2] = g;
}

int Background::getPixel(int x, int y)
{
	x &= 0xFF;
	y &= 0xFF;
	int pos = y*256+x;
	u16 g = gfx[pos/2];
	if(pos % 2 == 0)
		return g & 0xFF;
	else
		return g>>8;
}

void Background::setScroll(int x, int y)
{
	bgSetScroll(id, x, y);
	bgUpdate();
}

void Background::fill(int color)
{
	color |= color<<8;
	
	for(int i = 0; i < 256*256/2; i++)
		gfx[i] = color;
}

void Background::fillRect(int color, int x0, int y0, int x1, int y1)
{
	for(int x = x0; x < x1; x++)
		for(int y = y0; y < y1; y++)
			setPixel(x, y, color);
}

void Background::print(const char* text, Font& f, int color, int xx, int yy)
{
	int i = 0;
	while(text[i] != 0)
	{
		const u8* tile = f.data->data + text[i]*f.tileLength;
		int pixelsLeft = 8;
		int byte = *tile;
		
		for(int y = 0; y < f.tileSize; y++)
			for(int x = 0; x < f.tileSize; x++)
			{
				if(pixelsLeft == 0)
				{
					pixelsLeft = 8;
					tile++;
					byte = *tile;
				}

				pixelsLeft--;
				if(byte & 1)
					setPixel(x+xx, y+yy, color);
				byte = byte>>1;
			}
			
		xx += f.data->widths[text[i]];
		i++;
	}
}


TiledBackground::TiledBackground(){}
TiledBackground::TiledBackground(int id, int offset, int tileOffset)
{
	this->id = id;
	if(id < 4)
		bgInit(id, BgType_Text8bpp, BgSize_T_256x256, tileOffset, offset);
	else
		bgInitSub(id-4, BgType_Text8bpp, BgSize_T_256x256, tileOffset, offset);
		
	gfx = bgGetGfxPtr(id);
	map = bgGetMapPtr(id);

	clear();
	bgWrapOn(id);
	bgShow(id);	
}

void TiledBackground::clear()
{
	for(int i = 0; i< 32*32; i++)
		map[i] = 0;
}

void TiledBackground::setTile(int x, int y, int val)
{
	x %= 32;
	y %= 32;
	map[x+y*32] = val;
}

int TiledBackground::getTile(int x, int y)
{
	x %= 32;
	y %= 32;
	return map[x+y*32];
}

void TiledBackground::setScroll(int x, int y)
{
	bgSetScroll(id, x, y);
	bgUpdate();	
}
