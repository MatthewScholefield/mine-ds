#pragma once
void createChest(WorldObject &world, int x, int y, bool bg);
int getChestID(WorldObject &world, int x, int y, bool bg);
void destroyChest(WorldObject &world, int x, int y, bool bg);
void openChest(WorldObject &world, int x, int y, bool bg);
void closeChest();
int getOpenedChestID();
void saveChests(FILE *file, WorldObject &world);
void loadChests(FILE *file, WorldObject &world);