#pragma once

#include <string>

class MessageSystem {
public:
    void print(const std::string &message);
    void update();
    void clear();

private:
    void updateDisplay();
    int getOldestIndex();

    static constexpr int numMessages = 3;
    std::string messages[numMessages]{};
    int triggerTime = 1;
    bool mustUpdate = false;
};
