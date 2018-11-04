
#include "World.hpp"
#include "../graphics/MainRenderer.hpp"
#include "State.hpp"
#include "SimplexNoise.hpp"

void World::update() {
}

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
            blocks[x][y] = state->getBlock();
        }
    }
}

void World::render(MainRenderer &renderer) {
    auto &cam = renderer.getCam();
    renderer.setScroll(int(cam.x), int(cam.y));
    const int blockX = int(cam.x) / blockSize;
    const int blockY = int(cam.y) / blockSize;
    for (int renderX = blockX; renderX <= blockX + Graphics::px / blockSize; ++renderX) {
        for (int renderY = blockY; renderY <= blockY + Graphics::py / blockSize; ++renderY) {
            renderer.renderBlock(renderX, renderY, blocks[renderX][renderY]);
        }
    }
}
