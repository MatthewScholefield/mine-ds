/* 
 * File:   button.h
 * Author: Matthew
 *
 * Created on June 25, 2014, 6:28 PM
 */
#include <stdio.h>
#ifndef BUTTON_H
#define	BUTTON_H

class Button {
public:
    int length;
    int x, y;
    bool isColored;
    bool isTouching(touchPosition touch);
    void setColored(bool);
    void setDefault();
    Button(int , int , const char * const ); //Constructor
    Button(int , int , const char * const , int); //Constructor
};


#endif	/* BUTTON_H */

