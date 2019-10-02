#pragma once

class ADPCMProcessor {
public:
    char encode(signed long sample);
    signed long decode(char code);
    void reset();

private:
    signed long prevsample = 0;
    int previndex = 0;
    static const int imaStepTable[];
    static const int imaIndexTable[];
};
