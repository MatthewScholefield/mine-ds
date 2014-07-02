#ifndef BUTTON_H
#define	BUTTON_H

class Button {
public:
    int length;
    int x, y;
    bool isColored, visible;
    bool isTouching(int xVal, int yVal);
    void setVisible(bool);
    void setColored(bool);
    void draw();
    Button(int , int , const char * const ); //Constructor
    Button(int , int , const char * const , bool);
    Button(int , int , const char * const , int);
    Button(int , int , const char * const , int, bool);
    void initDefault(int,int,int,int,int,const char * const, bool);
    int printX, printY;
    const char *label;
};


#endif	/* BUTTON_H */

