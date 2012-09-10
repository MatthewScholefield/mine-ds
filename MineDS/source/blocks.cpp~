/*
        ----------------------------------------
        |                                       |
        |               Blocks.cpp              |
        |        General Block Handling         |
        -----------------------------------------
*/
#include "blockID.h"
#define sizeOfArray(x) (sizeof(x)/4)
int walkThroughBlocks[]={LOG,OAK_WOOD,BIRCH_WOOD,LEAF,YELLOW_FLOWER,RED_FLOWER,AIR,CACTUS,TORCH,SNOW_TOP,REDWOOD_LEAF};
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
int isAGroundBlock(int blockID)
{
        return false;
}
