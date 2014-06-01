#include <string>
void openFiles();
void closeFiles();
bool endOfFile();
std::string getLine();
const char getChar();
void stopNow();
void saveWorld(worldObject* world);
void loadWorld(worldObject* world);
