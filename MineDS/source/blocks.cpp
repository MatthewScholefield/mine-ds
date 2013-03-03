/*
        ----------------------------------------
        |                                       |
        |               Blocks.cpp              |
        |        General Block Handling         |
        -----------------------------------------
*/
#include "blockID.h"
#define sizeOfArray(x) (sizeof(x)/4)
int walkThroughBlocks[]={AIR,YELLOW_FLOWER,RED_FLOWER,CACTUS,TORCH,SNOW_TOP};
int renderBright[]={AIR,LOG,OAK_WOOD,BIRCH_WOOD,LEAF,YELLOW_FLOWER,RED_FLOWER,CACTUS,TORCH,REDWOOD_LEAF,GLASS,SNOW_TOP};
int lightSourceBlocks[]={TORCH,PUMPKIN_LIGHT,GLOWSTONE};
#define NUM_OF_LIGHT_SOURCES 3
//The Number is equal to 15 - minecraftlightemitvalue
int sourceAmount[]={1,0,0};
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
int alwaysRenderBright(int blockID)
{
        int i;
        bool isWalkThrough=false;
        for(i=0;i<=sizeOfArray(renderBright);i++)
        {
                if (renderBright[i]==blockID) isWalkThrough=true;
        }
        return isWalkThrough;
}
int isAGroundBlock(int blockID)
{
        return false;
}
bool isBlockALightSource(int blockID)
{
	int i;
        bool isLightSource=false;
        for(i=0;i<NUM_OF_LIGHT_SOURCES;i++)
        {
                if (lightSourceBlocks[i]==blockID) isLightSource=true;
        }
        return isLightSource;
}
int getLightAmount(int blockID)
{
	int i;
        bool isLightSource=false;
        for(i=0;i<NUM_OF_LIGHT_SOURCES;i++)
        {
                if (lightSourceBlocks[i]==blockID) return sourceAmount[i];
        }
}
