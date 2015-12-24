#pragma once
#include "WorldObject.h"

bool isCreative(void);
bool isSurvival(void);
void newGame(GameMode mode);
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