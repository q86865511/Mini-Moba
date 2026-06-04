#pragma once
#include "shared/Match.h"
#include "shared/entities/Hero.h"
#include "AssetManager.h"
#include "GameCamera.h"
#include "Renderer.h"
#include "EntityView.h"
#include "Effects.h"
#include "Input.h"
#include "Audio.h"

// Top-level application object: owns the match and all client modules, and runs
// the fixed-timestep game loop. main() just creates one and calls Run().
class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    static constexpr int   kScreenW = 1280;
    static constexpr int   kScreenH = 720;
    static constexpr float kWorldW  = 2048.0f;
    static constexpr float kWorldH  = 2048.0f;

    shared::Match match_;
    shared::Hero* hero_ = nullptr;

    AssetManager assets_;
    GameCamera   camera_{ kScreenW, kScreenH };
    Renderer     renderer_{ kScreenW, kScreenH, kWorldW, kWorldH };
    ViewRegistry views_;
    Effects      effects_;
    Input        input_;
    Audio        audio_;

    float accumulator_ = 0.0f;
};
