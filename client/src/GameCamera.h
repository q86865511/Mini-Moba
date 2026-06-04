#pragma once
#include "raylib.h"
#include "shared/Vec2.h"

// Wraps raylib's Camera2D: follows a target and keeps the view inside the world.
class GameCamera {
public:
    GameCamera(int screenWidth, int screenHeight);

    void Follow(shared::Vec2 targetPos, float worldWidth, float worldHeight);

    Camera2D Raw() const { return cam_; }
    Vector2 ScreenToWorld(Vector2 screenPos) const;

private:
    Camera2D cam_{};
    int screenW_;
    int screenH_;
};
