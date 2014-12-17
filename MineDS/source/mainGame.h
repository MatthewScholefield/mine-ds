#ifndef _MAIN_GAME_H_
#define _MAIN_GAME_H_

#include "world.h"

bool isCreative(void);
bool isSurvival(void);
void newGame(gamemode_t mode);
bool loadGame(void);
void previewScreen(int generate);
void startGame(void);
void startMultiplayerGame(bool host);
void returnToGame(void);

#endif /* !_MAIN_GAME_H_ */
