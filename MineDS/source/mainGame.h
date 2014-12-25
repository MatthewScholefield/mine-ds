#pragma once
#include "world.h"

bool isCreative(void);
bool isSurvival(void);
void newGame(gamemode_t mode, bool setSeed = true);
bool loadGame(void);
void previewScreen(int generate);
void startGame(void);
void startMultiplayerGame(bool host);
void returnToGame(void);
void setSeed(int seed);
