#pragma once
/* _____________________________________________
  | 						|
  |		    worldgen.h			|
  |     Part of Mine DS , by CoolAs and Ray	|
  |		Thanks to Dirbaio!		|
  |_____________________________________________|
 */

#include "world.h"
void fillWorld(worldObject* world, int blockType);
void generateWorld(worldObject* world);
void generateSmallWorld(worldObject* world);
void randGenerate(worldObject* world);
void fixgrass(worldObject* world);

