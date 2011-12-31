#include <nds.h>
u16 colours[]={
	RGB15(0,0,0),//0
	RGB15(0,31,0),//1
	RGB15(14,10,4),//2
	RGB15(28,24,0),//3
	RGB15(0,16,0),//4
	RGB15(0,0,31),//5
	RGB15(0,0,16),//6
	RGB15(0,0,24),//7	
	RGB15(24,16,16),//8
	RGB15(16,8,8),//9
	RGB15(31,4,4),//10
	RGB15(28,24,8),//11
	RGB15(24,1,20)//12
};
u8 theTile[64];
#define AIR 0
#define GRASS 1
#define DIRT 2
#define SAND 3
#define DARK_GRASS 4
#define LIGHT_WATER 5
#define DARK_WATER 6
#define WATER 7
#define STONE 8
#define DARK_STONE 9
#define LAVA 10
#define PLATE 11
#define DOOR 12
int  bg;
bool buffer;
u8 tiles[64] =
{
//Tile 0: transparent tile
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0	
};
void RenderInit(){  
 	int i;
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);
	for(i=0;i<=12;i++){
		int b;
		for (b=0;b<=64;b++){
			tiles[b]=i;		
		}
		dmaCopy(tiles, bgGetGfxPtr(bg)+32*i, sizeof(tiles));	
	}
	dmaCopy(colours, BG_PALETTE, sizeof(colours));
}
void RenderTile(int x,int y,char tile){
	x=x*2;
	y=y*2;
	u16* mapMemory=bgGetMapPtr(bg);
	mapMemory[x+(y*32)]=tile;
	mapMemory[x+(y*32)+1]=tile;
	mapMemory[x+(y*32)+32]=tile;
	mapMemory[x+(y*32)+33]=tile;
}
void RenderDraw(){
}
