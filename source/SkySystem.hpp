#pragma once

#include <nds/ndstypes.h>

class SkySystem {
public:
    SkySystem();
    void setSkyColor(double red1, double green1, double blue1, double red2, double green2, double blue2);

private:
    const bool useGradient = true, useDithering = true;
    uint16 backdropColor[192];
    double gradientData[192][3];
};
