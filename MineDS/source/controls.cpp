#include <nds.h>
#include <stdio.h>
#include "controls.h"
#include "general.h"

KEYPAD_BITS left = KEY_LEFT;
KEYPAD_BITS right = KEY_RIGHT;
KEYPAD_BITS jump = KEY_A;
KEYPAD_BITS crouch = KEY_DOWN;
KEYPAD_BITS itemLeft = KEY_X;
KEYPAD_BITS itemRight = KEY_B;
KEYPAD_BITS switchScreen = KEY_Y;
KEYPAD_BITS menu = KEY_START;
KEYPAD_BITS climb = KEY_UP;

KEYPAD_BITS getKey(int action)
{
	KEYPAD_BITS returnKey;
	switch (action)
	{
		case ACTION_MOVE_LEFT: returnKey = left;
			break;
		case ACTION_MOVE_RIGHT: returnKey = right;
			break;
		case ACTION_JUMP: returnKey = jump;
			break;
		case ACTION_CROUCH: returnKey = crouch;
			break;
		case ACTION_ITEM_LEFT: returnKey = itemLeft;
			break;
		case ACTION_ITEM_RIGHT: returnKey = itemRight;
			break;
		case ACTION_SWITCH_SCREEN: returnKey = switchScreen;
			break;
		case ACTION_MENU: returnKey = menu;
			break;
		case ACTION_CLIMB: returnKey = climb;
			break;
		default:
		{
			while (1)
			{
				iprintf("\x1b[%d;%dH  Invalid Return key!  ", rand() % 20, rand() % 30);
				for (int i = 0; i < 20; i++)
					swiWaitForVBlank();
			}
		}
	}
	return returnKey;
}

void setKey(int action, KEYPAD_BITS key)
{
	switch (action)
	{
		case ACTION_MOVE_LEFT: left = key;
			break;
		case ACTION_MOVE_RIGHT: right = key;
			break;
		case ACTION_JUMP: jump = key;
			break;
		case ACTION_CROUCH: crouch = key;
			break;
		case ACTION_ITEM_LEFT: itemLeft = key;
			break;
		case ACTION_ITEM_RIGHT: itemRight = key;
			break;
		case ACTION_SWITCH_SCREEN: switchScreen = key;
			break;
		case ACTION_MENU: menu = key;
			break;
		case ACTION_CLIMB: climb = key;
			break;
	}
}