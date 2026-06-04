#pragma once
#include "shared/World.h"
#include "shared/entities/Hero.h"

class AssetManager;
class GameCamera;

// Draws the world (map + entities) and a minimal HUD. Knows about raylib;
// the simulation (shared) never does.
class Renderer {
public:
    Renderer(int screenWidth, int screenHeight, float worldWidth, float worldHeight)
        : screenW_(screenWidth), screenH_(screenHeight),
          worldW_(worldWidth), worldH_(worldHeight) {}

    void Draw(const shared::World& world, const AssetManager& assets,
              const GameCamera& camera, const shared::Hero* playerHero);

private:
    void DrawGround(const AssetManager& assets);
    void DrawEntities(const shared::World& world, const AssetManager& assets);

    int   screenW_;
    int   screenH_;
    float worldW_;
    float worldH_;
};
