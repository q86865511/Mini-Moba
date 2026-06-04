#include "Input.h"
#include "GameCamera.h"
#include "raylib.h"

InputCommand Input::Process(shared::Hero* hero, const GameCamera& camera) {
    InputCommand cmd;
    if (!hero) return cmd;

    const Vector2 w = camera.ScreenToWorld(GetMousePosition());
    cmd.worldMouse = { w.x, w.y };

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) hero->SetMoveTarget(cmd.worldMouse);
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cmd.moved = true;

    if (IsKeyPressed(KEY_Q)) cmd.castQ = true;
    if (IsKeyPressed(KEY_B)) cmd.toggleShop = true;

    if (IsKeyPressed(KEY_ONE))   cmd.buyIndex = 0;
    else if (IsKeyPressed(KEY_TWO))   cmd.buyIndex = 1;
    else if (IsKeyPressed(KEY_THREE)) cmd.buyIndex = 2;
    else if (IsKeyPressed(KEY_FOUR))  cmd.buyIndex = 3;

    return cmd;
}
