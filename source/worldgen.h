#pragma once
/* _____________________________________________
  | 						|
  |		    worldgen.h			|
  |     Part of Mine DS , by CoolAs and Ray	|
  |		Thanks to Dirbaio!		|
  |_____________________________________________|
 */

#include "world.h"
void fillWorld(WorldObject* world, int blockType);
void generateWorld(WorldObject* world);
void generateSmallWorld(WorldObject* world);
void randGenerate(WorldObject* world);
void fixgrass(WorldObject* world);

