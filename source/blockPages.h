#pragma once
#include <nds.h>
#define NUM_BLOCK_PAGES 5
#define PAGE_WOOL 0
#define PAGE_BLOCKS 1
#define PAGE_PLANTS 2
#define PAGE_ORES 3
#define PAGE_TOOLS 4

void setBlockPage(int page);
int getBlockPage();
const char *getPageName(int page);
int pageMenuUpdate(touchPosition* touch, unsigned char* oldX, unsigned char* oldY, unsigned int* oldKeys);
void pageMenuInit();
