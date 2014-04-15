#include <nds.h>
#define ACTION_MOVE_LEFT 1
#define ACTION_MOVE_RIGHT 2
#define ACTION_JUMP 3
#define ACTION_CROUCH 4
#define ACTION_ITEM_LEFT 5
#define ACTION_ITEM_RIGHT 6
#define ACTION_SWITCH_SCREEN 7
#define ACTION_MENU 8
#define ACTION_CLIMB 9
KEYPAD_BITS getKey(int action);
void setKey(int action, KEYPAD_BITS key);