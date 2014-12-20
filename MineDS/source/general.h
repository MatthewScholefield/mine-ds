#ifndef _GENERAL_H_
#define _GENERAL_H_

#define FPS		(60)
#define SEC_TO_FPS(S)	((S) * FPS)

void sleep(unsigned int seconds);
void print_message(const char* s);
void update_message();
void show_message(const char* s);
void clear_messages();
void printXY(int x, int y, const char *output);
void printXY(int x, int y, int output);
void updateTime();
unsigned int getTime();

#endif /* _GENERAL_H_ */
