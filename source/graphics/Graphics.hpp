#pragma once

#include <nds/ndstypes.h>
#include <string>
#include "SkySystem.hpp"
#include "../FileSystem.hpp"
#include "Texture.hpp"


class SubRenderer;

class Graphics {
public:
    static constexpr int px = 256, py = 192;

    Graphics();
    void bind(SubRenderer &titleGraphics);
    void beginRender(int screenX, int screenY);

    int getMainBgID() const;
    int getSubBgID() const;
    const Texture &getTexture() const;
    SkySystem getSkySystem();

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
    SubRenderer *titleGraphics;
};
