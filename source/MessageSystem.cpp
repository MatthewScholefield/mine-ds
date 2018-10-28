#include "MessageSystem.hpp"
#include "utils.hpp"

int MessageSystem::getOldestIndex() {
    for (int i = numMessages - 1; i >= 0; --i) {
        if (messages[i].empty()) {
            return i == numMessages - 1 ? i : i + 1;
        }
    }
    return 0;
}

void MessageSystem::print(const std::string &message) {
    for (int i = 0; i < numMessages - 1; ++i) {
        messages[i] = messages[i + 1];
    }
    messages[numMessages - 1] = message;
    if (getOldestIndex() == numMessages - 1) { //the first message should reset the trigger time
        triggerTime = getTime() % MESSAGE_CLEAR_DELAY - 1; //Prevent message instantly dissapearing
        if (triggerTime < 0) {
            triggerTime = MESSAGE_CLEAR_DELAY - 1;
        }
    }
}

void MessageSystem::update() {
    if (getTime() % MESSAGE_CLEAR_DELAY == unsigned(triggerTime)) {
        messages[getOldestIndex()] = "";
    }
    for (int amount = 0; amount < numMessages; ++amount) {
        printXY(1, 20 + amount, messages[amount]);
    }
}

void MessageSystem::clear() {
    for (auto &message : messages) {
        message = "";
    }
    update();
}
