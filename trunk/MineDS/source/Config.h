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


#ifndef CONFIG_H
#define	CONFIG_H

class Config {
public:
    KEYPAD_BITS left;
    KEYPAD_BITS right;
    KEYPAD_BITS jump;
    KEYPAD_BITS crouch;
    KEYPAD_BITS itemLeft;
    KEYPAD_BITS itemRight;
    KEYPAD_BITS switchScreen;
    KEYPAD_BITS menu;
    KEYPAD_BITS climb;

    void setKey(int action, KEYPAD_BITS key);
    KEYPAD_BITS getKey(int action);
    Config ():left(KEY_LEFT),
    right(KEY_RIGHT),
    jump(KEY_A),
    crouch(KEY_DOWN),
    itemLeft(KEY_X),
    itemRight(KEY_B),
    switchScreen(KEY_Y),
    menu(KEY_START),
    climb(KEY_UP)
    {
        
    }
};

#endif	/* CONFIG_H */