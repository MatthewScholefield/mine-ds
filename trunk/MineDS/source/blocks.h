#define NUM_SPRITE_BLOCKS 4

bool isBlockWalkThrough(int a);
bool isAGroundBlock(int a);
int getLightAmount(int a);
bool isBlockALightSource(int a);
bool item(int a);
bool alwaysRenderBright(int a);
void initBlockProperties();
int getHardness(int blockID);
int getType(int blockID);
int getSpriteBlock (int index);
bool isSpriteBlock(int block);