#pragma once

#include <stdio.h>


template<size_t bufferSize>
class NibbleReader {
public:
    NibbleReader(FILE *file = nullptr) : file(file), buffer{}, c{}, bufferPos(bufferSize), nibblePos(true) {}
    
    int get() {
        nibblePos = !nibblePos;
        if (nibblePos) {
            return (c & 0xf0) >> 4;
        } else {
            c = getCharacter();
            return c & 0x0f;
        }
    }
    
    bool isDone() {
        return bufferPos > leftInBuffer;
    }
    
    void reset() {
        bufferPos = bufferSize;
        nibblePos = true;
    }
    
    void setFile(FILE *file) {
        this->file = file;
        reset();
    }
    
private:
    char getCharacter() {
        if (bufferPos > leftInBuffer) {  // No more data in file
            return 0;
        }
        if (bufferPos >= bufferSize) {
            leftInBuffer = fread(buffer, 1, bufferSize, file);
            bufferPos = 0;
        }
        return buffer[bufferPos++];
    }
    
    FILE *file{};
    char buffer[bufferSize]{}, c{};
    unsigned int bufferPos = bufferSize, leftInBuffer = bufferSize;
    bool nibblePos = true;
};
