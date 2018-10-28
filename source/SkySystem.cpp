#include "SkySystem.hpp"
#include "utils.hpp"

uint16 *currentColors;

static void gradientHandler() {
    int r = REG_VCOUNT + 1;
    if (r < 0) r = 191;
    if (r >= 192) r = 0;
    BG_PALETTE[0] = currentColors[r];
}

SkySystem::SkySystem() {
    currentColors = this->backdropColor;
    irqSet(IRQ_HBLANK, gradientHandler);
    irqEnable(IRQ_HBLANK);
}

void SkySystem::setSkyColor(double red1, double green1, double blue1, double red2, double green2, double blue2) {
    if (useGradient) {
        for (int i = 0; i < 192; ++i) {
            gradientData[i][0] = min(red1 * (1.0 - i / 191.0) + (red2 * i) / 191, 31.0);
            gradientData[i][1] = min(green1 * (1.0 - i / 191.0) + (green2 * i) / 191, 31.0);
            gradientData[i][2] = min(blue1 * (1.0 - i / 191.0) + (blue2 * i) / 191, 31.0);
        }
        if (useDithering) {
            for (u16 i = 0; i < 191; ++i) {
                for (u8 j = 0; j < 3; ++j) {
                    double extra = gradientData[i][j] - ((int) gradientData[i][j]);
                    gradientData[i][j] = int(gradientData[i][j]);
                    gradientData[i + 1][j] += extra;
                }
            }
            for (u8 i = 0; i < 3; ++i) {
                gradientData[191][i] = int(gradientData[191][i]);
            }
        }
        for (u16 i = 0; i < 192; ++i) {
            backdropColor[i] = (uint16) RGB15(int(gradientData[i][0]), int(gradientData[i][1]),
                                              int(gradientData[i][2]));
        }
    } else {
        const auto ra = roundInt((red1 + red2) / 2.0);
        const auto ga = roundInt((green1 + green2) / 2.0);
        const auto ba = roundInt((blue1 + blue2) / 2.0);
        for (uint16 &i : backdropColor) {
            i = (uint16) RGB15(ra, ga, ba);
        }
    }
}
