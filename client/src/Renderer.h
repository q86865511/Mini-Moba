#pragma once
#include "raylib.h"
#include "shared/World.h"
#include "shared/entities/Hero.h"
#include "EntityView.h"
#include "Effects.h"
#include "Shop.h"
#include <vector>
#include <string>

class AssetManager;
class GameCamera;

// Draws the world (map + entities + health bars), effects, HUD, minimap, shop.
class Renderer {
public:
    Renderer(int screenWidth, int screenHeight, float worldWidth, float worldHeight)
        : screenW_(screenWidth), screenH_(screenHeight),
          worldW_(worldWidth), worldH_(worldHeight) {}

    void Draw(const shared::World& world, const AssetManager& assets,
              const ViewRegistry& views, const GameCamera& camera,
              const shared::Hero* playerHero, const Effects& effects,
              int matchResult, const Shop& shop);

    void DrawMenu(const AssetManager& assets);
    void DrawHeroSelect(const std::vector<std::string>& heroes, int selected, const AssetManager& assets);

private:
    void DrawGround(const AssetManager& assets);
    void DrawEntity(const shared::Entity& e, const AssetManager& assets, const ViewRegistry& views);
    void DrawHud(const shared::Hero* hero);
    void DrawMinimap(const shared::World& world, const AssetManager& assets, const GameCamera& camera);
    void DrawShopPanel(const Shop& shop, const shared::Hero* hero);

    int   screenW_;
    int   screenH_;
    float worldW_;
    float worldH_;
};
