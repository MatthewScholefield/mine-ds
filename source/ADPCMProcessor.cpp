#include "ADPCMProcessor.hpp"

#include <iostream>

const int ADPCMProcessor::imaStepTable[89] = {
        7, 8, 9, 10, 11, 12, 13, 14,
        16, 17, 19, 21, 23, 25, 28, 31,
        34, 37, 41, 45, 50, 55, 60, 66,
        73, 80, 88, 97, 107, 118, 130, 143,
        157, 173, 190, 209, 230, 253, 279, 307,
        337, 371, 408, 449, 494, 544, 598, 658,
        724, 796, 876, 963, 1060, 1166, 1282, 1411,
        1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
        3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
        7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
        15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
        32767
};

const int ADPCMProcessor::imaIndexTable[16] = {
        -1, -1, -1, -1, 2, 4, 6, 8,
        -1, -1, -1, -1, 2, 4, 6, 8
};

char ADPCMProcessor::encode(signed long sample) {
    signed long predsample; /* Output of ADPCM predictor */

    int code; // ADPCM output value

    predsample = prevsample;
    int index = previndex;
    int step = imaStepTable[index];  // Quantizer step size

    // Difference between sample and predicted sample
    signed long diff = sample - predsample;
    if (diff >= 0) {
        code = 0;
    } else {
        code = 8;
        diff = -diff;
    }

    // Quantize the difference into the 4-bit ADPCM code using the the quantizer step size
    int tempstep = step;
    if (diff >= tempstep) {
        code |= 4;
        diff -= tempstep;
    }
    tempstep >>= 1;
    if (diff >= tempstep) {
        code |= 2;
        diff -= tempstep;
    }
    tempstep >>= 1;
    if (diff >= tempstep) {
        code |= 1;
    }

    // Inverse quantize the ADPCM code into a predicted difference using the quantizer step size
    signed long diffq = step >> 3;  // Dequantized predicted difference
    if (code & 4) {
        diffq += step;
    }
    if (code & 2) {
        diffq += step >> 1;
    }
    if (code & 1) {
        diffq += step >> 2;
    }

    if (code & 8) {
        predsample -= diffq;
    } else {
        predsample += diffq;
    }

    if (predsample > 32767) {  // Check for overflow
        predsample = 32767;
    } else if (predsample < -32768) {
        predsample = -32768;
    }

    index += imaIndexTable[code];
    if (index < 0) {  // Check for overflow
        index = 0;
    }
    if (index > 88) {
        index = 88;
    }

    prevsample = predsample;
    previndex = index;

    return char(code & 0x0f);  // Return the new ADPCM code
}

signed long ADPCMProcessor::decode(char code) {
    long step = imaStepTable[previndex];  // Quantizer step size. Find quantizer step size from lookup table using index

    // Inverse quantize the ADPCM code into a difference using the quantizer step size
    signed long diffq = step >> 3;;  // Dequantized predicted difference
    if (code & 4) {
        diffq += step;
    }
    if (code & 2) {
        diffq += step >> 1;
    }
    if (code & 1) {
        diffq += step >> 2;
    }

    if (code & 8) {  // Add the difference to the predicted sample
        prevsample -= diffq;
    } else {
        prevsample += diffq;
    }

    if (prevsample > 32767) {  // Correct overflow
        prevsample = 32767;
    } else if (prevsample < -32768) {
        prevsample = -32768;
    }

    previndex += imaIndexTable[int(code)];

    // Check for overflow of the new quantizer step size index
    if (previndex < 0) {
        previndex = 0;
    }
    if (previndex > 88) {
        previndex = 88;
    }
    return prevsample;
}

void ADPCMProcessor::reset() {
    prevsample = previndex = 0;
}
