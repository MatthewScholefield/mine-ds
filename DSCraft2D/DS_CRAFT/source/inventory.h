#include "world.h"
typedef struct{
	int blockID[36];
	int blockAmount[36];
}inventoryStruct;
int inventoryAdd(int blockID);
int inventoryRemove(int blockID);
void inventoryInit();
void DrawAmount(worldObject* world);
void DrawAmountNum(int i);
