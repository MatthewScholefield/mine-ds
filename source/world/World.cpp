
#include <ctime>
#include <queue>

#include "World.hpp"
#include "../graphics/MainRenderer.hpp"
#include "State.hpp"
#include "SimplexNoise.hpp"
#include "../graphics/Graphics.hpp"

void World::update() {}

void World::generate() {
    ObjectChooser<Biome,
            Entry<(int) Biome::PLAINS, 15>,
            Entry<(int) Biome::SNOW, 10>,
            Entry<(int) Biome::JUNGLE, 0>
    > biomeChooser;

    const int RANGE = 20;
    float frequency = 25.0f / sx;
    for (int x = 0; x < sx; ++x) {
        State *state = State::air;

        float biomeNoise = SimplexNoise::noise((float) x * frequency / 3.f);
        biomeNoise = 0.5f * biomeNoise + 0.5f;
        Biome biome = biomeChooser.choose(int(biomeNoise * biomeChooser.getSum()));
        for (int y = 0; y < sy; ++y) {
            float smallNoise = SimplexNoise::noise((float) x * frequency, (float) y * frequency);
            float medNoise = SimplexNoise::noise((float) x * frequency / 6.f, (float) y * frequency / 6.f);

            float threshold = (y - sy / 2.f) / RANGE;
            if (threshold < -1.f)
                threshold = -1.f;
            else if (threshold > 1.f)
                threshold = 1.f;

            float noiseSum = (2.f * medNoise + 0.2f * smallNoise) / 2.2f;

            State *newState = state->update(noiseSum, -threshold, biome);
            if (newState != state) {
                state = newState;
                state->enter();
            }
            fg[x][y] = state->getBlock();
        }
    }
    calculateBrightness();
}

const int SUB_AMOUNT[16] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3};

void World::brightnessUpdate(int x, int y, int level) {
    if ((unsigned) x >= sx || (unsigned) y >= sy)
        return;
    int before = brightness[x][y];
    if (level > before) {
        brightness[x][y] = level;
        checkBlock(x, y);
    }
}

void World::checkBlock(int x, int y) {
    int level = brightness[x][y] - (isWalkThrough(fg[x][y]) ? 1 : SUB_AMOUNT[brightness[x][y]]);
    brightnessUpdate(x + 1, y, level);
    brightnessUpdate(x - 1, y, level);
    brightnessUpdate(x, y + 1, level);
    brightnessUpdate(x, y - 1, level);
}

void World::calculateBrightness() {
    calculateBrightness(0, sx - 1, 0, sy - 1);
}

void World::calculateBrightness(int x, int y) {
    calculateBrightness(x - 8, x + 8, y - 6, y + 6);
}

struct Light {
    int x, y, brightness;

    Light(int x, int y, int brightness) : x(x), y(y), brightness(brightness) {}

    bool operator<(const struct Light &other) const
    {
        return brightness < other.brightness;
    }
};

void World::calcBrightnessDefault(int leftBound, int rightBound, int topBound, int bottomBound) {
    const int maxSpread = 7;
    const int minX = max(0, leftBound - maxSpread);
    const int maxX = min(sx - 1, rightBound + maxSpread);
    const int minY = max(0, topBound - maxSpread);
    const int maxY = min(sy - 1, bottomBound + maxSpread);
    for (int i = minX; i <= maxX; ++i) {
        bool startedShade = false;
        for (int j = 0; j <= maxY; ++j) {
            Block block = fg[i][j];
            if (!startedShade) {
                brightness[i][j] = sunBrightness;
                if (!isWalkThrough(block)) {
                    startedShade = true;
                }
            } else if (block == Block::Air && bg[i][j] == Block::Air)
                brightness[i][j] = sunBrightness / 2;
            else {
                brightness[i][j] = 0;
            }
            int emission = getLightEmission(block);
            if (emission) {
                brightness[i][j] = max(brightness[i][j], emission);
            }
        }
    }
    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j <= maxY; ++j) {
            checkBlock(i, j);
        }
    }
}

void World::calcBrightnessModified(int leftBound, int rightBound, int topBound, int bottomBound) {
    const int maxSpread = 7;
    const int minX = max(0, leftBound - maxSpread);
    const int maxX = min(sx - 1, rightBound + maxSpread);
    const int minY = max(0, topBound - maxSpread);
    const int maxY = min(sy - 1, bottomBound + maxSpread);

    std::queue<Light> lights;
    for (int i = minX; i <= maxX; ++i) {
        bool startedShade = false;
        for (int j = 0; j <= maxY; ++j) {
            Block block = fg[i][j];
            int blockBrightness = 0;
            if (!startedShade) {
                blockBrightness = sunBrightness;
                if (!isWalkThrough(block)) {
                    startedShade = true;
                }
            } else if (block == Block::Air && bg[i][j] == Block::Air) {
                blockBrightness = sunBrightness / 2;
            }
            if (j >= minY) {
                brightness[i][j] = 0;
                int emission = getLightEmission(block);
                if (emission) {
                    blockBrightness = max(blockBrightness, emission);
                }
                if (blockBrightness != 0) {
                    lights.emplace(i, j, blockBrightness);
                }
            }
        }
    }
    while (!lights.empty()) {
        auto light = lights.front();
        lights.pop();
        auto &slot = brightness[light.x][light.y];
        if (light.brightness > slot) {
            slot = light.brightness;
            int allDeltas[][2] = {
                {1, 0},
                {0, 1},
                {-1, 0},
                {0, -1}
            };
            int level = slot - (isWalkThrough(fg[light.x][light.y]) ? 1 : SUB_AMOUNT[slot]);
            for (auto deltas : allDeltas) {
                int nx = light.x + deltas[0];
                int ny = light.y + deltas[1];
                if ((unsigned)nx < sx && (unsigned)ny < sy && level > brightness[nx][ny]) {
                    lights.emplace(nx, ny, level);
                }
            }
        }
    }
}

void World::calculateBrightness(int leftBound, int rightBound, int topBound, int bottomBound) {
    static int counter = 0;
    cpuStartTiming(0);
    if (counter >= 2) {
        calcBrightnessDefault(leftBound, rightBound, topBound, bottomBound);
    } else {
        calcBrightnessModified(leftBound, rightBound, topBound, bottomBound);
    }
    u32 diff = cpuEndTiming();
	printXY(1, 8, "%s: %u   ", counter >= 2 ? " Default" : "Modified", diff);
    ++counter;
    if (counter >= 4) {
        counter = 0;
    }
}

void World::render(MainRenderer &renderer) {
    auto &cam = renderer.getCam();
    const int blockX = int(cam.x);
    const int blockY = int(cam.y);
    for (int renderX = blockX; renderX <= blockX + Graphics::px / Graphics::blockSize; ++renderX) {
        for (int renderY = blockY; renderY <= blockY + Graphics::py / Graphics::blockSize; ++renderY) {
            renderer.renderBlock({renderX, renderY}, fg[renderX][renderY], brightness[renderX][renderY]);
        }
    }
}
