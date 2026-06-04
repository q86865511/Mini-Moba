#include "Renderer.h"
#include "AssetManager.h"
#include "GameCamera.h"
#include <vector>
#include <algorithm>
#include <string>
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
        default:                 return Color{ 200, 190, 90, 255 };
    }
}

void Renderer::Draw(const shared::World& world, const AssetManager& assets,
                    const ViewRegistry& views, const GameCamera& camera,
                    const shared::Hero* playerHero, const Effects& effects,
                    int matchResult, const Shop& shop) {
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

    // --- HUD (screen space) ---
    DrawText("RIGHT: move   Q: ability   B: shop   ESC: quit", 14, 12, 18, RAYWHITE);
    DrawFPS(screenW_ - 90, screenH_ - 24);

    if (playerHero) {
        const bool ready = playerHero->qTimer <= 0.0f;
        char buf[32];
        if (ready) snprintf(buf, sizeof(buf), "Q  [READY]");
        else       snprintf(buf, sizeof(buf), "Q  [%.1fs]", playerHero->qTimer);
        DrawRectangle(14, screenH_ - 46, 150, 32, Color{ 0, 0, 0, 150 });
        DrawText(buf, 24, screenH_ - 40, 22, ready ? Color{ 120, 230, 150, 255 } : Color{ 220, 200, 120, 255 });
    }

    DrawHud(playerHero);
    DrawMinimap(world, assets, camera);
    if (shop.open && matchResult == 0) DrawShopPanel(shop, playerHero);

    if (matchResult != 0) {
        DrawRectangle(0, 0, screenW_, screenH_, Color{ 0, 0, 0, 140 });
        const char* msg = (matchResult == 1) ? "VICTORY" : "DEFEAT";
        const Color col = (matchResult == 1) ? Color{ 120, 230, 150, 255 } : Color{ 235, 90, 80, 255 };
        const int fs = 84;
        const int w = MeasureText(msg, fs);
        DrawText(msg, screenW_ / 2 - w / 2, screenH_ / 2 - fs / 2, fs, col);
        const char* sub = "Press ESC to quit";
        const int sw = MeasureText(sub, 24);
        DrawText(sub, screenW_ / 2 - sw / 2, screenH_ / 2 + fs / 2 + 12, 24, RAYWHITE);
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

void Renderer::DrawHud(const shared::Hero* h) {
    if (!h) return;
    char buf[80];

    // Gold (top-right)
    snprintf(buf, sizeof(buf), "Gold: %d", h->gold);
    DrawRectangle(screenW_ - 200, 10, 188, 30, Color{ 0, 0, 0, 150 });
    DrawText(buf, screenW_ - 190, 16, 22, Color{ 255, 220, 90, 255 });

    // Level + XP bar (bottom-center)
    const int bw = 320, bh = 16;
    const int bx = screenW_ / 2 - bw / 2, by = screenH_ - 30;
    DrawRectangle(bx - 1, by - 1, bw + 2, bh + 2, Color{ 0, 0, 0, 170 });
    DrawRectangle(bx, by, bw, bh, Color{ 40, 40, 55, 255 });
    float frac = h->XpToNext() > 0.0f ? h->xp / h->XpToNext() : 0.0f;
    if (frac < 0) frac = 0; if (frac > 1) frac = 1;
    DrawRectangle(bx, by, (int)(bw * frac), bh, Color{ 120, 180, 255, 255 });
    snprintf(buf, sizeof(buf), "Lv %d", h->level);
    DrawText(buf, bx - 52, by - 3, 22, RAYWHITE);

    // Items (just above the xp bar)
    if (!h->items.empty()) {
        std::string s = "Items: ";
        for (size_t i = 0; i < h->items.size(); ++i) {
            s += h->items[i];
            if (i + 1 < h->items.size()) s += ", ";
        }
        const int tw = MeasureText(s.c_str(), 16);
        DrawText(s.c_str(), screenW_ / 2 - tw / 2, by - 24, 16, Color{ 200, 220, 200, 255 });
    }
}

void Renderer::DrawMinimap(const shared::World& world, const AssetManager& assets, const GameCamera& camera) {
    const int mm = 210, pad = 12;
    const int mx = screenW_ - mm - pad, my = screenH_ - mm - pad - 50;
    DrawRectangle(mx - 2, my - 2, mm + 4, mm + 4, Color{ 0, 0, 0, 180 });
    const Texture2D map = assets.Map();
    DrawTexturePro(map, { 0, 0, (float)map.width, (float)map.height },
                   { (float)mx, (float)my, (float)mm, (float)mm }, Vector2{ 0, 0 }, 0.0f,
                   Color{ 255, 255, 255, 220 });

    const float sc = (float)mm / worldW_;
    for (auto& e : world.entities) {
        if (!e || !e->alive || e->Type() == shared::EntityType::Projectile) continue;
        const int dx = mx + (int)(e->pos.x * sc), dy = my + (int)(e->pos.y * sc);
        int r = 2;
        if (e->Type() == shared::EntityType::Hero) r = 4;
        else if (e->Type() == shared::EntityType::Nexus || e->Type() == shared::EntityType::Tower) r = 3;
        DrawCircle(dx, dy, (float)r, TeamColor(e->team));
    }

    const Camera2D cam = camera.Raw();
    const float vw = screenW_ / cam.zoom, vh = screenH_ / cam.zoom;
    const float vx = cam.target.x - vw / 2.0f, vy = cam.target.y - vh / 2.0f;
    DrawRectangleLines(mx + (int)(vx * sc), my + (int)(vy * sc),
                       (int)(vw * sc), (int)(vh * sc), Color{ 255, 255, 255, 200 });
}

void Renderer::DrawShopPanel(const Shop& shop, const shared::Hero* h) {
    const int pw = 440, ph = 320;
    const int px = screenW_ / 2 - pw / 2, py = screenH_ / 2 - ph / 2;
    DrawRectangle(0, 0, screenW_, screenH_, Color{ 0, 0, 0, 120 });
    DrawRectangle(px, py, pw, ph, Color{ 24, 28, 38, 245 });
    DrawRectangleLines(px, py, pw, ph, Color{ 120, 140, 180, 255 });

    DrawText("SHOP", px + 16, py + 14, 26, RAYWHITE);
    DrawText("(B to close)", px + 90, py + 20, 18, Color{ 170, 170, 180, 255 });
    char buf[64];
    snprintf(buf, sizeof(buf), "Gold: %d", h ? h->gold : 0);
    DrawText(buf, px + pw - 140, py + 16, 22, Color{ 255, 220, 90, 255 });

    const auto& items = shop.Items();
    for (size_t i = 0; i < items.size(); ++i) {
        const int iy = py + 60 + (int)i * 60;
        const bool afford = h && h->gold >= items[i].cost;
        snprintf(buf, sizeof(buf), "[%d]  %s  -  %dg", (int)i + 1, items[i].name.c_str(), items[i].cost);
        DrawText(buf, px + 18, iy, 22, afford ? RAYWHITE : Color{ 130, 130, 130, 255 });
        DrawText(items[i].desc.c_str(), px + 18, iy + 26, 16, Color{ 160, 200, 160, 255 });
    }
}
