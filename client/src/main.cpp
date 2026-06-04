// Phase 0: prove the toolchain works.
// Opens a window and draws a square that follows the mouse cursor.
// (Default raylib font is ASCII-only, so on-screen text stays English.)

#include "raylib.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Mini MOBA - Phase 0");
    SetTargetFPS(60);

    const float boxSize = 40.0f;

    while (!WindowShouldClose()) // ESC or window close button
    {
        const Vector2 mouse = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Phase 0: window + a box that follows the mouse. Press ESC to quit.",
                 20, 20, 20, DARKGRAY);

        // Center the box on the mouse position.
        const Rectangle box = {
            mouse.x - boxSize / 2.0f,
            mouse.y - boxSize / 2.0f,
            boxSize,
            boxSize
        };
        DrawRectangleRec(box, MAROON);
        DrawCircleV(mouse, 4.0f, DARKBLUE);

        DrawFPS(screenWidth - 90, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
