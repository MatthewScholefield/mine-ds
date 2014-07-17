#include <string>
bool openFiles();
bool closeFiles();
bool endOfFile();
std::string getLine();
const char getChar();
void stopNow();
bool saveWorld(worldObject* world);
void loadWorld(worldObject* world);
bool saveControls(bool silent = false);
bool loadControls(bool silent = false);
