#include "world.h"
#include <nds.h>
#define NUM_BLOCKS 65
int getSelectedblock();
void miningUpdate(worldObject* world,int sx,int sy,touchPosition touch, int keys);
void blocksCanPlace();
void miningSetScene(bool a);
