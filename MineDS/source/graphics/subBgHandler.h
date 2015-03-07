#pragma once
void subBgInit();
void setSubBgTile(int x, int y, int tile);
void setSubBgTile(int x, int y, int tile, int flip);
void drawButton(int x, int y, int sizex);
void drawButtonColored(int x, int y, int sizex);
void drawBox(int x, int y, int lx, int ly);
void drawBoxCenter(int x, int y, int lx, int ly);
#define V_FLIP 2
#define H_FLIP 1
#define BOTH_FLIP 3
