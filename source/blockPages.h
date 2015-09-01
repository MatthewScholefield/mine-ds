#pragma once
#define NUM_BLOCK_PAGES 6
#define PAGE_WOOL 0
#define PAGE_BLOCKS 1
#define PAGE_CRAFTING 2
#define PAGE_PLANTS 3
#define PAGE_ORES 4
#define PAGE_TOOLS 5

const char *getPageName();
void updatePageName();
void setBlockPage(int page);
void changeBlockPage(bool forward, bool skipUpdate = false);