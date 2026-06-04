#include "Input.h"
#include "GameCamera.h"
#include "raylib.h"

bool Input::Process(shared::Hero* hero, const GameCamera& camera) {
    if (!hero) return false;

    // Hold/click the RIGHT mouse button to walk toward the cursor (LoL style).
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        const Vector2 w = camera.ScreenToWorld(GetMousePosition());
        hero->SetMoveTarget({ w.x, w.y });
    }

    return IsMouseButtonPressed(MOUSE_BUTTON_RIGHT); // fresh click -> move cue
}
