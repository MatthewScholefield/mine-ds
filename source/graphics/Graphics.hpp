#pragma once

#include <nds/ndstypes.h>
#include <string>
#include <bits/unique_ptr.h>
#include "SkySystem.hpp"
#include "../FileSystem.hpp"
#include "Texture.hpp"
#include "Font.hpp"


class SubRenderer;

class Graphics {
public:
    static constexpr int px = 256, py = 192, blockSize = 16;

    Graphics();

    int getMainBgID() const;
    int getSubBgID() const;
    const Texture &getTexture() const;
    SkySystem &getSkySystem();
    Font &getFont();
private:
    void updateTexture();
    bool loadTextureFile(const std::string &fileName, bool reloadGfx = true);
    void setBlockPalette(bool blocks, int brightness, int index);

    template<typename T>
    void dmaCopyVec(const std::vector<T> &data, void *dest) {
        dmaCopy(data.data(), dest, uint32(data.size() * sizeof(T)));
    }

    Texture texture;
    int subBgID, mainBgID;
    SkySystem sky;
    std::unique_ptr<Font> font;
};
