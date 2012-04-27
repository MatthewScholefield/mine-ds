/*
        ----------------------------------------
        |                                       |
        |               Blocks.cpp              |
        |        General Block Handling         |
        -----------------------------------------
*/
#include "blockID.h"
#define sizeOfArray(x) (sizeof(x)/4)
int walkThroughBlocks[]={LOG,OAK_WOOD,BIRCH_WOOD,LEAF,FLOWER_YELLOW,FLOWER_RED,AIR,CACTUS,TORCH,SNOW_TOP};
int isBlockWalkThrough(int blockID)
{
        int i;
        bool isWalkThrough=false;
        for(i=0;i<=sizeOfArray(walkThroughBlocks);i++)
        {
                if (walkThroughBlocks[i]==blockID) isWalkThrough=true;
        }
        return isWalkThrough;
}
