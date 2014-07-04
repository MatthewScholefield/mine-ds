#include <nds.h>

KEYPAD_BITS getControlKey(int);
void setControlKey(int, KEYPAD_BITS);
void saveControlData(FILE* data);
void loadControlData(FILE* data);