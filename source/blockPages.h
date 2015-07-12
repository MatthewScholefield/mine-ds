#pragma once
#include <nds.h>
#define NUM_BLOCK_PAGES 6
#define PAGE_WOOL 0
#define PAGE_BLOCKS 1
#define PAGE_CRAFTING 2
#define PAGE_PLANTS 3
#define PAGE_ORES 4
#define PAGE_TOOLS 5

void setBlockPage(int page);
int getBlockPage();
const char *getPageName(int page);
void updatePageName();
int pageMenuUpdate(touchPosition* touch);
void pageMenuInit();
void changeBlockPage(bool forward);