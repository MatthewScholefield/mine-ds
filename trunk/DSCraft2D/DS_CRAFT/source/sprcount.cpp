#include <nds.h>
//This Code keeps track of the sprites that are displayed on the screen, 
//It gives every sprite a different oamId and makes sure that there are no two sprites with the same oamId
//nextSprite(); gives the next oamId and returns it
//resetSpriteCount(); needs to be done once per frame and sets the oamID count to 0
int sprite;
void resetSpriteCount(){
	sprite=0;
	oamClear(&oamMain,0,128);
}
int nextSprite(){
	sprite++;
	return sprite;
}
