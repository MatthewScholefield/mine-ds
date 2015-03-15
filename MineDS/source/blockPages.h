#pragma once
#include <nds.h>
#define NUM_BLOCK_PAGES 3
#define PAGE_WOOL 0
#define PAGE_ORES 1
#define PAGE_TOOLS 2

void setBlockPage(int page);
int getBlockPage();
const char *getPageName(int page);
int pageMenuUpdate(touchPosition* touch, unsigned char* oldX, unsigned char* oldY, unsigned int* oldKeys);
void pageMenuInit();
