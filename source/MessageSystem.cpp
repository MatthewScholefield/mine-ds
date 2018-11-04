#include "MessageSystem.hpp"
#include "utils.hpp"
#include "graphics/Font.hpp"

int MessageSystem::getOldestIndex() {
    for (int i = numMessages - 1; i >= 0; --i) {
        if (messages[i].empty()) {
            return i == numMessages - 1 ? i : i + 1;
        }
    }
    return 0;
}

void MessageSystem::print(const std::string &message) {
    mustUpdate = true;
    for (int i = 0; i < numMessages - 1; ++i) {
        messages[i] = messages[i + 1];
    }
    messages[numMessages - 1] = message;
    if (getOldestIndex() == numMessages - 1) { //the first message should reset the trigger time
        triggerTime = counter % clearDelay - 1; //Prevent message instantly dissapearing
        if (triggerTime < 0) {
            triggerTime = clearDelay - 1;
        }
    }
}

void MessageSystem::update() {
    ++counter;
    if (counter % clearDelay == triggerTime) {
        messages[getOldestIndex()] = "";
        mustUpdate = true;
    }
    if (mustUpdate) {
        updateDisplay();
    }
}

void MessageSystem::clear() {
    for (auto &message : messages) {
        message = "";
    }
    update();
}

void MessageSystem::updateDisplay() {
    mustUpdate = false;
    for (int msgId = 0; msgId < numMessages; ++msgId) {
        printXY(1, 20 + msgId, messages[msgId]);
        for (int i = 1 + int(messages[msgId].size()); i < Font::cx; ++i) {
            printXY(i, 20 + msgId, " ");
        }
    }
}
