#pragma once

#include "graphics/Graphics.hpp"
#include "graphics/SubRenderer.hpp"
#include "graphics/MainRenderer.hpp"
#include "SoundSystem.hpp"
#include "MessageSystem.hpp"
#include "world/World.hpp"
#include "entities/Player.hpp"


class Game {
public:
    Graphics graphics;
    SubRenderer subRenderer;
    MainRenderer mainRenderer;
    SoundSystem soundSystem;
    MessageSystem messageSystem;
    std::unique_ptr<World> world;
    Player player;
    
    Game();
    void update();
};