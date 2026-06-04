#include "Renderer.h"
#include "AssetManager.h"
#include "GameCamera.h"

static Vector2 ToScreen(shared::Vec2 v) { return Vector2{ v.x, v.y }; }

void Renderer::Draw(const shared::World& world, const AssetManager& assets,
                    const GameCamera& camera, const shared::Hero* playerHero,
                    Rectangle heroFrame) {
    BeginDrawing();
    ClearBackground(Color{ 12, 14, 18, 255 });

    BeginMode2D(camera.Raw());
    DrawGround(assets);

    // Move-destination marker.
    if (playerHero && playerHero->hasTarget) {
        const Vector2 t = ToScreen(playerHero->moveTarget);
        DrawCircleLines((int)t.x, (int)t.y, 14.0f, Color{ 90, 220, 140, 255 });
        DrawCircleLines((int)t.x, (int)t.y, 5.0f, Color{ 90, 220, 140, 255 });
    }

    // Hero: animated sprite. The frame's anchor [128,205] (feet) aligns to its position.
    if (playerHero) {
        const Vector2 p = ToScreen(playerHero->pos);
        const Texture2D sheet = assets.HeroSheet();
        if (sheet.id != 0 && heroFrame.width > 0.0f) {
            const float S = 160.0f; // on-screen frame size
            const Rectangle dst = { p.x, p.y, S, S };
            const Vector2 origin = { 0.5f * S, (205.0f / 256.0f) * S };
            DrawTexturePro(sheet, heroFrame, dst, origin, 0.0f, WHITE);
        } else {
            DrawCircleV(p, playerHero->radius, SKYBLUE);
        }
    }
    EndMode2D();

    // HUD (screen space).
    DrawText("Phase 2: hold RIGHT mouse to move. Camera follows. ESC to quit.",
             16, 16, 20, RAYWHITE);
    DrawFPS(screenW_ - 90, 16);

    EndDrawing();
}

void Renderer::DrawGround(const AssetManager& assets) {
    const Texture2D map = assets.Map();
    // Stretch the map (or the fallback tile) to fill the whole world.
    const Rectangle src = { 0, 0, (float)map.width, (float)map.height };
    const Rectangle dst = { 0, 0, worldW_, worldH_ };
    DrawTexturePro(map, src, dst, Vector2{ 0, 0 }, 0.0f, WHITE);
    DrawRectangleLines(0, 0, (int)worldW_, (int)worldH_, Color{ 40, 48, 64, 255 });
}
