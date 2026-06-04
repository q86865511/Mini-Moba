#pragma once
#include "raylib.h"
#include "shared/World.h"
#include "shared/entities/Hero.h"
#include "EntityView.h"
#include "Effects.h"

class AssetManager;
class GameCamera;

// Draws the world (map + entities + health bars), effects, and a minimal HUD.
class Renderer {
public:
    Renderer(int screenWidth, int screenHeight, float worldWidth, float worldHeight)
        : screenW_(screenWidth), screenH_(screenHeight),
          worldW_(worldWidth), worldH_(worldHeight) {}

    void Draw(const shared::World& world, const AssetManager& assets,
              const ViewRegistry& views, const GameCamera& camera,
              const shared::Hero* playerHero, const Effects& effects,
              int matchResult); // 0=ongoing, 1=victory, 2=defeat

private:
    void DrawGround(const AssetManager& assets);
    void DrawEntity(const shared::Entity& e, const AssetManager& assets, const ViewRegistry& views);

    int   screenW_;
    int   screenH_;
    float worldW_;
    float worldH_;
};
