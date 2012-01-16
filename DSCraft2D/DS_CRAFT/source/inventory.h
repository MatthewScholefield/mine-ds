#include "world.h"
#include <stdio.h>
typedef struct{
	int blockID[36];
	int blockAmount[36];
}inventoryStruct;
int inventoryAdd(int blockID);
int inventoryRemove(int blockID);
void inventoryInit();
void DrawAmount(worldObject* world);
void DrawAmountNum(int i);
void invLoad(FILE* save_file);
void invSave(FILE* save_file);
bool invHave(int blockID);
int inventoryAddAmount(int i,int j);
