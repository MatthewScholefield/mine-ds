#pragma once

#include <nds/ndstypes.h>
#include <string>
#include "SkySystem.hpp"
#include "../FileSystem.hpp"
#include "Texture.hpp"


class SubRenderer;

class GraphicsSystem {
public:
    GraphicsSystem();
    void bind(SubRenderer &titleGraphics);
    void beginRender(int screenX, int screenY);

    uint16 *getMainBgPtr() const;
    uint16 *getSubBgPtr() const;
    int getSubBgID() const;
    const Texture &getTexture() const;

private:
    void updateTexture();
    bool loadTextureFile(const std::string &fileName, bool reloadGfx = true);
    void setBlockPalette(bool blocks, int brightness, int index);

    template<typename T>
    void dmaCopyVec(const std::vector<T> &data, void *dest) {
        dmaCopy(data.data(), dest, uint32(data.size() * sizeof(T)));
    }

    Texture texture;
    uint16 *subBgPtr, *mainBgPtr;
    int subBgID, mainBgID;
    SkySystem sky;
    SubRenderer *titleGraphics;
};
