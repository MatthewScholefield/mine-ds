#include <nds.h>
#include "Config.h"

KEYPAD_BITS getControlKey(int);
Config* getSettings();
void setControlKey(int, KEYPAD_BITS);
void setHerobrineOn(bool on);
bool getHerobrineOn();
void setDrawMode(bool on);
bool getDrawMode();