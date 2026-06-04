#include "Renderer.h"
#include "AssetManager.h"
#include "GameCamera.h"
#include <vector>
#include <algorithm>
#include <cstdio>

static Vector2 ToScreen(shared::Vec2 v) { return Vector2{ v.x, v.y }; }

static float DisplaySize(const shared::Entity& e) {
    switch (e.Type()) {
        case shared::EntityType::Hero:           return 150.0f;
        case shared::EntityType::Minion:         return 84.0f;
        case shared::EntityType::NeutralMonster: return 150.0f;
        case shared::EntityType::Tower:          return 230.0f;
        case shared::EntityType::Nexus:          return 270.0f;
        case shared::EntityType::Projectile:     return 40.0f;
        default:                                 return 100.0f;
    }
}

static Color TeamColor(shared::Team t) {
    switch (t) {
        case shared::Team::Blue: return Color{ 70, 150, 255, 255 };
        case shared::Team::Red:  return Color{ 230, 70, 70, 255 };
        default:                 return Color{ 200, 200, 90, 255 };
    }
}

void Renderer::Draw(const shared::World& world, const AssetManager& assets,
                    const ViewRegistry& views, const GameCamera& camera,
                    const shared::Hero* playerHero, const Effects& effects) {
    BeginDrawing();
    ClearBackground(Color{ 12, 14, 18, 255 });

    BeginMode2D(camera.Raw());
    DrawGround(assets);

    if (playerHero && playerHero->hasTarget) {
        const Vector2 t = ToScreen(playerHero->moveTarget);
        DrawCircleLines((int)t.x, (int)t.y, 14.0f, Color{ 90, 220, 140, 255 });
        DrawCircleLines((int)t.x, (int)t.y, 5.0f, Color{ 90, 220, 140, 255 });
    }

    std::vector<const shared::Entity*> ents;
    ents.reserve(world.entities.size());
    for (auto& e : world.entities) if (e) ents.push_back(e.get());
    std::sort(ents.begin(), ents.end(),
              [](const shared::Entity* a, const shared::Entity* b) { return a->pos.y < b->pos.y; });
    for (const auto* e : ents) DrawEntity(*e, assets, views);

    effects.DrawWorld();
    EndMode2D();

    // HUD
    DrawText("hold RIGHT mouse: move   |   Q: ability   |   ESC: quit", 16, 16, 20, RAYWHITE);
    DrawFPS(screenW_ - 90, 16);
    if (playerHero) {
        const bool ready = playerHero->qTimer <= 0.0f;
        char buf[32];
        if (ready) snprintf(buf, sizeof(buf), "Q  [READY]");
        else       snprintf(buf, sizeof(buf), "Q  [%.1fs]", playerHero->qTimer);
        DrawRectangle(14, screenH_ - 46, 150, 32, Color{ 0, 0, 0, 150 });
        DrawText(buf, 24, screenH_ - 40, 22, ready ? Color{ 120, 230, 150, 255 } : Color{ 220, 200, 120, 255 });
    }

    EndDrawing();
}

void Renderer::DrawGround(const AssetManager& assets) {
    const Texture2D map = assets.Map();
    const Rectangle src = { 0, 0, (float)map.width, (float)map.height };
    const Rectangle dst = { 0, 0, worldW_, worldH_ };
    DrawTexturePro(map, src, dst, Vector2{ 0, 0 }, 0.0f, WHITE);
    DrawRectangleLines(0, 0, (int)worldW_, (int)worldH_, Color{ 40, 48, 64, 255 });
}

void Renderer::DrawEntity(const shared::Entity& e, const AssetManager& assets, const ViewRegistry& views) {
    const Vector2 p = ToScreen(e.pos);
    const float S = DisplaySize(e);

    const EntityView* v = views.Get(e.id);
    const AnimationSet* set = v ? v->player.Set() : nullptr;
    const Texture2D sheet = v ? assets.Sheet(v->key) : Texture2D{};

    bool drewSprite = false;
    if (v && set && sheet.id != 0) {
        const Rectangle fr = v->player.Frame();
        if (fr.width > 0.0f && fr.height > 0.0f) {
            const Rectangle dst = { p.x, p.y, S, S };
            const Vector2 origin = { (set->anchor.x / fr.width) * S, (set->anchor.y / fr.height) * S };
            const Color tint = e.alive ? WHITE : Color{ 170, 170, 170, 230 };
            DrawTexturePro(sheet, fr, dst, origin, 0.0f, tint);
            drewSprite = true;
        }
    }
    if (!drewSprite) {
        Color c = (e.Type() == shared::EntityType::Projectile) ? Color{ 255, 235, 130, 255 } : TeamColor(e.team);
        DrawCircleV(p, e.radius, c);
    }

    if (e.alive && e.maxHp > 0.0f && e.Type() != shared::EntityType::Projectile) {
        const float w = S * 0.42f, h = 6.0f;
        const float x = p.x - w / 2.0f, y = p.y - S * 0.52f;
        float frac = e.hp / e.maxHp;
        if (frac < 0) frac = 0; if (frac > 1) frac = 1;
        DrawRectangle((int)x - 1, (int)y - 1, (int)w + 2, (int)h + 2, Color{ 0, 0, 0, 170 });
        DrawRectangle((int)x, (int)y, (int)w, (int)h, Color{ 60, 24, 24, 255 });
        DrawRectangle((int)x, (int)y, (int)(w * frac), (int)h, TeamColor(e.team));
    }
}
