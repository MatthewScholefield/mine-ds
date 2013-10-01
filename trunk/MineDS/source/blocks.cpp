/*
        -----------------------------------------
        |               Blocks.cpp              |
        |        General Block Handling         |
        -----------------------------------------
*/
#include "blockID.h"
#include "mining.h"
#define sizeOfArray(x) (sizeof(x)/4)
int walkThroughBlocks[]={AIR,YELLOW_FLOWER,RED_FLOWER,SNOW_TOP,TORCH,LADDER,SHRUB,TALL_GRASS};
int renderBright[]={AIR,LOG,OAK_WOOD,BIRCH_WOOD,LEAF,YELLOW_FLOWER,RED_FLOWER,CACTUS,TORCH,REDWOOD_LEAF,GLASS,SNOW_TOP,SHRUB,TALL_GRASS};
int lightSourceBlocks[]={TORCH,PUMPKIN_LIGHT,GLOWSTONE};
int items[]={PORKCHOP_RAW,BEEF_RAW,LEATHER,PICKAXE_WOOD,PICKAXE_STONE,
		PICKAXE_IRON,PICKAXE_GOLD,PICKAXE_DIAMOND,COAL,IRON,GOLD,DIAMOND,
		STICK,FLESH,BEEF_COOKED,PORKCHOP_COOKED,CHICKEN_RAW,CHICKEN_COOKED};
int hardness[NUM_BLOCKS]; //Slot is ID number, negative number means tool
/*
----TOOL----
-Negative value
-Faster Mining = Farther from 0
-Slower Mining = Closer to 0

----Block---
-Positive Value
-Harder Object = Higher Value
-Softer Object = Lower value (Min 0)

	 DIRT ~ 1
	STONE ~ 20
TREE WOOD ~ 3
*/

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
int item(int blockID)
{
        int i;
        bool isItem=false;
        for(i=0;i<=sizeOfArray(items);i++)
        {
                if (items[i]==blockID) isItem=true;
        }
        return isItem;
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
void initHardness()
{
	int i;
	for (i=1;i<=NUM_BLOCKS;i++)
		hardness[i]=1;
	for (i=1;i<=NUM_BLOCKS;i++)
		if (item(i))
			hardness[i]=-2;
	hardness[PICKAXE_GOLD]=-20;
	hardness[PICKAXE_DIAMOND]=-16;
	hardness[PICKAXE_IRON]=-10;
	hardness[PICKAXE_STONE]=-6;
	hardness[PICKAXE_WOOD]=-4;
	hardness[STONE]=20;
	hardness[BIRCH_WOOD]=3;
	hardness[OAK_WOOD]=3;

}
int getHardness(int blockID)
{
	return hardness[blockID];
}
