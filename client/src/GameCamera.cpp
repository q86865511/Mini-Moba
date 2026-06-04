#include "GameCamera.h"

GameCamera::GameCamera(int screenWidth, int screenHeight)
    : screenW_(screenWidth), screenH_(screenHeight) {
    cam_.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    cam_.target = { 0.0f, 0.0f };
    cam_.rotation = 0.0f;
    cam_.zoom = 1.0f;
}

void GameCamera::Follow(shared::Vec2 targetPos, float worldWidth, float worldHeight) {
    const float halfW = screenW_ / 2.0f;
    const float halfH = screenH_ / 2.0f;

    float cx = targetPos.x;
    float cy = targetPos.y;

    // Clamp so we never show outside the world bounds.
    if (worldWidth > screenW_) {
        if (cx < halfW) cx = halfW;
        if (cx > worldWidth - halfW) cx = worldWidth - halfW;
    } else {
        cx = worldWidth / 2.0f;
    }
    if (worldHeight > screenH_) {
        if (cy < halfH) cy = halfH;
        if (cy > worldHeight - halfH) cy = worldHeight - halfH;
    } else {
        cy = worldHeight / 2.0f;
    }

    cam_.target = { cx, cy };
}

Vector2 GameCamera::ScreenToWorld(Vector2 screenPos) const {
    return GetScreenToWorld2D(screenPos, cam_);
}
