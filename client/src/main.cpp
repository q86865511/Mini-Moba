// Phase 1: run the shared simulation locally and move a hero by right-clicking.
// The simulation (shared::World) is pure logic with NO raylib dependency, so the
// server can reuse it unchanged in phase 2. The client only feeds input and draws.

#include "raylib.h"
#include "shared/World.h"

// Sim space -> screen space. Phase 1 has no camera, so this is 1:1.
static Vector2 ToScreen(shared::Vec2 v) { return Vector2{ v.x, v.y }; }

static void DrawBackgroundGrid(int width, int height, int spacing, Color line)
{
    for (int x = 0; x <= width; x += spacing) DrawLine(x, 0, x, height, line);
    for (int y = 0; y <= height; y += spacing) DrawLine(0, y, width, y, line);
}

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Mini MOBA - Phase 1");
    SetTargetFPS(60);

    shared::World world;
    world.hero.pos = { screenWidth / 2.0f, screenHeight / 2.0f };

    float accumulator = 0.0f;

    while (!WindowShouldClose())
    {
        // --- Input: hold/press the RIGHT mouse button to set the destination ---
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            const Vector2 m = GetMousePosition();
            world.SetMoveTarget({ m.x, m.y });
        }

        // --- Fixed-timestep simulation ---
        accumulator += GetFrameTime();
        if (accumulator > 0.25f) accumulator = 0.25f; // avoid spiral of death on a hitch
        while (accumulator >= shared::kFixedDt)
        {
            world.Tick(shared::kFixedDt);
            accumulator -= shared::kFixedDt;
        }

        // --- Render ---
        BeginDrawing();
        ClearBackground(Color{ 28, 28, 38, 255 });
        DrawBackgroundGrid(screenWidth, screenHeight, 64, Color{ 45, 45, 60, 255 });

        // Destination marker (where the hero is walking to).
        if (world.hero.hasTarget)
        {
            const Vector2 t = ToScreen(world.hero.moveTarget);
            DrawCircleLines((int)t.x, (int)t.y, 10.0f, Color{ 90, 200, 120, 255 });
            DrawCircleLines((int)t.x, (int)t.y, 3.0f, Color{ 90, 200, 120, 255 });
        }

        // Hero.
        const Vector2 hp = ToScreen(world.hero.pos);
        DrawCircleV(hp, 16.0f, SKYBLUE);
        DrawCircleLines((int)hp.x, (int)hp.y, 16.0f, DARKBLUE);

        // HUD.
        DrawText("Phase 1: hold RIGHT mouse on the ground to move. ESC to quit.",
                 20, 20, 20, RAYWHITE);
        DrawFPS(screenWidth - 90, 20);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
