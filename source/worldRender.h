#pragma once
/* _____________________________________________
  | 						|
  |		WorldRender.h			|
  |     Part of Mine DS , by CoolAs and Ray	|
  |		Thanks to Dirbaio!		|
  |_____________________________________________|
 */

#include "world.h"
#include "block_small.h"

void worldRender_Init();
void worldRender_Render(WorldObject* world, int screen_x, int screen_y);
void Calculate_Brightness(WorldObject* world);
extern "C" {
	void brightnessSpread(WorldObject* world, int x, int y, int brightness);
	void sunSpread(WorldObject* world, int x, int y, int brightness);
}
void updateBrightnessAround(WorldObject* world, int x, int y);
void worldRender_LoadSprites();
void setSun(int brightness);
int getBrightness(WorldObject* world, int x, int y);
