#pragma once
void createChest(World &world, int x, int y, bool bg);
int getChestID(World &world, int x, int y, bool bg);
void destroyChest(World &world, int x, int y, bool bg);
void openChest(World &world, int x, int y, bool bg);
void closeChest();
int getOpenedChestID();
void saveChests(FILE *file, World &world);
void loadChests(FILE *file, World &world);