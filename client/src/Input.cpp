#include "Input.h"
#include "GameCamera.h"
#include "raylib.h"

InputCommand Input::Process(shared::Hero* hero, const GameCamera& camera) {
    InputCommand cmd;
    if (!hero) return cmd;

    const Vector2 w = camera.ScreenToWorld(GetMousePosition());
    cmd.worldMouse = { w.x, w.y };

    // Hold/click RIGHT mouse to walk toward the cursor.
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) hero->SetMoveTarget(cmd.worldMouse);
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cmd.moved = true;

    // Q: cast the ability toward the cursor.
    if (IsKeyPressed(KEY_Q)) cmd.castQ = true;

    return cmd;
}
