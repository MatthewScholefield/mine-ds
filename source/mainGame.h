#pragma once
#include "world.h"

bool isCreative(void);
bool isSurvival(void);
void newGame(gamemode_t mode, int seed);
void previewGame(void);
bool loadGame(void);
void joinGame(void);
void startGame(void);
void startMultiplayerGame(bool host);
void returnToGame(void);
void setSeed(int seed);
void quitGame();
bool songIsPlaying();
void drawWorld();