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
void worldRender_Render(worldObject* world, int screen_x, int screen_y);
void Calculate_Brightness(worldObject* world);
extern "C" {
        void brightnessSpread(worldObject* world,int x,int y,int brightness);
}
void updateBrightnessAround(worldObject* world,int x,int y);
