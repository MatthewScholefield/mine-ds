#define ROUND(a) int(a + 0.5)
#define CEILING(X) (X-(int)(X) > 0 ? (int)(X+1) : (int)(X))
#define ABS(a) (a)<0?(a)*-1:(a)
void print_message(const char* s);
void update_message();
void show_message(const char* s);
void clear_messages();
void printXY(int x, int y, const char *output);
void printXY(int x, int y, int output);