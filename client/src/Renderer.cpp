#include "Renderer.h"
#include "AssetManager.h"
#include "GameCamera.h"
#include "raylib.h"

static Vector2 ToScreen(shared::Vec2 v) { return Vector2{ v.x, v.y }; }

void Renderer::Draw(const shared::World& world, const AssetManager& assets,
                    const GameCamera& camera, const shared::Hero* playerHero) {
    BeginDrawing();
    ClearBackground(Color{ 18, 20, 26, 255 });

    BeginMode2D(camera.Raw());
    DrawGround(assets);

    // Move-destination marker.
    if (playerHero && playerHero->hasTarget) {
        const Vector2 t = ToScreen(playerHero->moveTarget);
        DrawCircleLines((int)t.x, (int)t.y, 12.0f, Color{ 90, 200, 120, 255 });
        DrawCircleLines((int)t.x, (int)t.y, 4.0f, Color{ 90, 200, 120, 255 });
    }

    DrawEntities(world, assets);
    EndMode2D();

    // HUD (screen space).
    DrawText("Phase 2: hold RIGHT mouse to move. Camera follows. ESC to quit.",
             16, 16, 20, RAYWHITE);
    DrawFPS(screenW_ - 90, 16);

    EndDrawing();
}

void Renderer::DrawGround(const AssetManager& assets) {
    const Texture2D tile = assets.Ground();
    const int ts = tile.width;
    for (int y = 0; y < (int)worldH_; y += ts) {
        for (int x = 0; x < (int)worldW_; x += ts) {
            DrawTexture(tile, x, y, WHITE);
        }
    }
    DrawRectangleLines(0, 0, (int)worldW_, (int)worldH_, Color{ 60, 70, 90, 255 });
}

void Renderer::DrawEntities(const shared::World& world, const AssetManager& assets) {
    for (const auto& e : world.entities) {
        if (!e || !e->alive) continue;
        const Vector2 p = ToScreen(e->pos);
        if (e->Type() == shared::EntityType::Hero) {
            const Texture2D tex = assets.Hero();
            DrawTextureV(tex, Vector2{ p.x - tex.width / 2.0f, p.y - tex.height / 2.0f }, WHITE);
        } else {
            DrawCircleV(p, e->radius, GRAY);
        }
    }
}
